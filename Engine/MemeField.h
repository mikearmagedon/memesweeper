#pragma once

#include "Graphics.h"
#include "RectI.h"

class MemeField
{
public:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
		void Draw(const Vei2 & rScreenPos, bool triggered, Graphics & rGfx) const;
		void SpawnMeme();
		bool HasMeme() const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighborMemeCount(const int memeCount);
	private:
		State mState = State::Hidden;
		bool mHasMeme = false;
		int mNNeighborMemes = -1;
	};
public:
	MemeField(const Vei2 center, int nMemes);
	void Draw(Graphics & rGfx) const;
	RectI GetRect() const;
	void OnRevealClick(const Vei2 & rScreenPos);
	void OnFlagClick(const Vei2 & rScreenPos);
private:
	Tile & TileAt(const Vei2 & rGridPos);
	const Tile & TileAt(const Vei2 & rGridPos) const;
	Vei2 ScreenToGrid(const Vei2 & rScreenPos) const;
	int CountNeighborMemes(const Vei2 & rGridPos);
private:
	Vei2 mTopLeft;
	static constexpr int mWidth = 20;
	static constexpr int mHeight = 20;
	Tile mField[mWidth * mHeight];
	bool mIsTriggered = false;
};

