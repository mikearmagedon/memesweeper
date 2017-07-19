#include "MemeField.h"
#include "Vei2.h"
#include "SpriteCodex.h"
#include <cassert>
#include <random>


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
}

void MemeField::Draw(Graphics & rGfx) const
{
	rGfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0, 0 }; gridPos.y < mHeight; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < mWidth; gridPos.x++)
		{
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, rGfx);
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI(0, mWidth * SpriteCodex::tileSize, 0, mHeight * SpriteCodex::tileSize);
}

MemeField::Tile & MemeField::TileAt(const Vei2 & rGridPos)
{
	return field[rGridPos.y * mWidth + rGridPos.x];
}

const MemeField::Tile & MemeField::TileAt(const Vei2 & rGridPos) const
{
	return field[rGridPos.y * mWidth + rGridPos.x];
}

void MemeField::Tile::Draw(const Vei2 & rScreenPos, Graphics & rGfx) const
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
			SpriteCodex::DrawTile0(rScreenPos, rGfx);
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

bool MemeField::Tile::HasMeme() const
{
	return mHasMeme;
}

void MemeField::Tile::SpawnMeme()
{
	assert(!mHasMeme);
	mHasMeme = true;
}
