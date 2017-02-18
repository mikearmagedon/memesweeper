#pragma once

class Vec2
{
public:
	Vec2() = default;
	Vec2( int x_in,int y_in );
	Vec2 operator+( const Vec2& rhs ) const;
	Vec2& operator+=( const Vec2& rhs );
	Vec2 operator*( int rhs ) const;
	Vec2& operator*=( int rhs );
	Vec2 operator-( const Vec2& rhs ) const;
	Vec2& operator-=( const Vec2& rhs );
	Vec2 operator/( int rhs ) const;
	Vec2& operator/=( int rhs );
	float GetLength() const;
	int GetLengthSq() const;
public:
	int x;
	int y;
};