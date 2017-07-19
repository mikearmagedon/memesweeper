#include "MemeField.h"
#include "Vei2.h"
#include "SpriteCodex.h"
#include <cassert>
#include <random>
#include <algorithm>

MemeField::MemeField(int nMemes)
{
	assert(nMemes > 0 && nMemes < mWidth * mHeight);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, mWidth - 1);
	std::uniform_int_distribution<int> yDist(0, mHeight - 1);

	for (int nSpawned = 0; nSpawned < nMemes; ++nSpawned)
	{
		Vei2 spawnPos;
		do
		{
			spawnPos = { xDist(rng), yDist(rng) };
		} while (TileAt(spawnPos).HasMeme());

		TileAt(spawnPos).SpawnMeme();
	}

	for (Vei2 gridPos = { 0, 0 }; gridPos.y < mHeight; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < mWidth; gridPos.x++)
		{
			TileAt(gridPos).SetNeighborMemeCount(CountNeighborMemes(gridPos));
		}
	}
}

void MemeField::Draw(Graphics & rGfx) const
{
	rGfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0, 0 }; gridPos.y < mHeight; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < mWidth; gridPos.x++)
		{
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, mIsTriggered, rGfx);
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI(0, mWidth * SpriteCodex::tileSize, 0, mHeight * SpriteCodex::tileSize);
}

void MemeField::OnRevealClick(const Vei2 & rScreenPos)
{
	if (!mIsTriggered)
	{
		const Vei2 gridPos = ScreenToGrid(rScreenPos);
		assert(gridPos.x >= 0 && gridPos.x < mWidth && gridPos.y >= 0 && gridPos.y < mHeight);
		Tile & rTile = TileAt(gridPos);
		if (!rTile.IsRevealed() && !rTile.IsFlagged())
		{
			rTile.Reveal();
			if (rTile.HasMeme())
			{
				mIsTriggered = true;
			}
		}
	}
}

void MemeField::OnFlagClick(const Vei2 & rScreenPos)
{
	if (!mIsTriggered)
	{
		const Vei2 gridPos = ScreenToGrid(rScreenPos);
		assert(gridPos.x > 0 && gridPos.x < mWidth && gridPos.y > 0 && gridPos.y < mHeight);
		Tile & rTile = TileAt(gridPos);
		if (!rTile.IsRevealed())
		{
			rTile.ToggleFlag();
		}
	}
}

MemeField::Tile & MemeField::TileAt(const Vei2 & rGridPos)
{
	return mField[rGridPos.y * mWidth + rGridPos.x];
}

const MemeField::Tile & MemeField::TileAt(const Vei2 & rGridPos) const
{
	return mField[rGridPos.y * mWidth + rGridPos.x];
}

Vei2 MemeField::ScreenToGrid(const Vei2 & rScreenPos) const
{
	return rScreenPos / SpriteCodex::tileSize;
}

int MemeField::CountNeighborMemes(const Vei2 & rGridPos)
{
	const int xStart = std::max(0, rGridPos.x - 1);
	const int yStart = std::max(0, rGridPos.y - 1);
	const int xEnd = std::min(mWidth - 1, rGridPos.x + 1);
	const int yEnd = std::min(mHeight - 1, rGridPos.y + 1);

	int count = 0;
	for (Vei2 gridPos = { xStart, yStart }; gridPos.y <= yEnd; gridPos.y++)
	{
		for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
		{
			if (TileAt(gridPos).HasMeme())
			{
				count++;
			}
		}
	}
	return count;
}

void MemeField::Tile::Draw(const Vei2 & rScreenPos, bool triggered, Graphics & rGfx) const
{
	if (!triggered)
	{
		switch (mState)
		{
		case MemeField::Tile::State::Hidden:
			SpriteCodex::DrawTileButton(rScreenPos, rGfx);
			break;
		case MemeField::Tile::State::Flagged:
			SpriteCodex::DrawTileButton(rScreenPos, rGfx);
			SpriteCodex::DrawTileFlag(rScreenPos, rGfx);
			break;
		case MemeField::Tile::State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(rScreenPos, mNNeighborMemes, rGfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(rScreenPos, rGfx);
			}
			break;
		default:
			break;
		}
	}
	else
	{
		switch (mState)
		{
		case MemeField::Tile::State::Hidden:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(rScreenPos, rGfx);
			}
			else
			{
				SpriteCodex::DrawTileButton(rScreenPos, rGfx);
			}
			break;
		case MemeField::Tile::State::Flagged:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(rScreenPos, rGfx);
				SpriteCodex::DrawTileFlag(rScreenPos, rGfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(rScreenPos, rGfx);
				SpriteCodex::DrawTileCross(rScreenPos, rGfx);
			}
			break;
		case MemeField::Tile::State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(rScreenPos, mNNeighborMemes, rGfx);
			}
			else
			{
				SpriteCodex::DrawTileBombRed(rScreenPos, rGfx);
			}
			break;
		default:
			break;
		}
	}
}

bool MemeField::Tile::HasMeme() const
{
	return mHasMeme;
}

bool MemeField::Tile::IsRevealed() const
{
	return mState == State::Revealed;
}

bool MemeField::Tile::IsFlagged() const
{
	return mState == State::Flagged;
}

void MemeField::Tile::SetNeighborMemeCount(const int memeCount)
{
	assert(mNNeighborMemes == -1);
	mNNeighborMemes = memeCount;
}

void MemeField::Tile::SpawnMeme()
{
	assert(!mHasMeme);
	mHasMeme = true;
}

void MemeField::Tile::Reveal()
{
	assert(mState == State::Hidden);
	mState = State::Revealed;
}

void MemeField::Tile::ToggleFlag()
{
	assert(!IsRevealed());
	if (mState == State::Hidden)
	{
		mState = State::Flagged;
	}
	else
	{
		mState = State::Hidden;
	}
}
