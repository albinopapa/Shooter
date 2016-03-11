#pragma once
struct Vec2
{
	Vec2();
	Vec2(float X, float Y);
	Vec2(Vec2 &&c);
	Vec2(const Vec2 &V);
	bool operator==(const Vec2 &c)const;
	Vec2 operator+(const Vec2 &V)const;
	Vec2 operator-(const Vec2 &V)const;
	Vec2 operator*(const float S)const;
	Vec2 operator/(const float S)const;
	
	Vec2 &operator+=(const Vec2 &V);
	Vec2 &operator-=(const Vec2 &V);
	Vec2 &operator*=(const float S);
	Vec2 &operator/=(const float S);
	Vec2 &operator=(Vec2 &&V);
	Vec2 &operator=(const Vec2 &V);

	float Dot(const Vec2 &V)const;
	float Length()const;
	Vec2 Slope()const;

	float x,y;
};

float CalcSpeed( float speed );

float CalcSlopeToAngle( float slope );
Vec2 CalcAngleToSlope( int angle );
float CalcDegToRad( int angleDegree );

Vec2 CalcScreenScale(const Vec2& scrn );
Vec2 ScreenRes(const Vec2& Coord, const Vec2& scrn );
Vec2 WorldToScreen(const Vec2& Coord, const Vec2& scrn, const Vec2& cam );
Vec2 ScreenToWorld(const Vec2& Coord, const Vec2& scrn, const Vec2& cam );
