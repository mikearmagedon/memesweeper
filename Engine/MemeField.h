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
		void Draw(const Vei2 & rScreenPos, Graphics & rGfx) const;
		bool HasMeme() const;
		void SpawnMeme();
	private:
		State mState = State::Hidden;
		bool mHasMeme = false;
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics & rGfx) const;
	RectI GetRect() const;
private:
	Tile & TileAt(const Vei2 & rGridPos);
	const Tile & TileAt(const Vei2 & rGridPos) const;
private:
	static constexpr int mWidth = 20;
	static constexpr int mHeight = 20;
	Tile field[mWidth * mHeight];
};

