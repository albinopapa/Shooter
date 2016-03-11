#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
	:
	x(0.0f),
	y(0.0f)
{
}

Vec2::Vec2(float X, float Y) 
	: 
	x(X), 
	y(Y) 
{}

Vec2::Vec2(Vec2 && c)
	:
	x(c.x),
	y(c.y)
{
	c.x = 0.0f;
	c.y = 0.0f;
}

Vec2::Vec2(const Vec2 & V)
	:
	x(V.x),
	y(V.y)
{
}

bool Vec2::operator==(const Vec2 & c) const
{
	return (((int)x == (int)c.x) && ((int)y == (int)c.y));
}

Vec2 Vec2::operator+(const Vec2 &V) const
{
	return Vec2(x + V.x, y + V.y);
}

Vec2 Vec2::operator-(const Vec2 &V) const
{
	return Vec2(x - V.x, y - V.y);
}

Vec2 Vec2::operator*(const float S) const
{
	return Vec2(x * S, y * S);
}

Vec2 Vec2::operator/(const float S) const
{
	return Vec2(x / S, y / S);
}

Vec2 & Vec2::operator+=(const Vec2 & V)
{
	x += V.x;
	y += V.y;
	return(*this);
}

Vec2 & Vec2::operator-=(const Vec2 & V)
{
	x -= V.x;
	y -= V.y;
	return (*this);
}

Vec2 & Vec2::operator*=(const float S)
{
	return *this = *this * S;
}

Vec2 & Vec2::operator/=(const float S)
{
	return (*this) = (*this) / S;
}

Vec2 & Vec2::operator=(Vec2 && V)
{
	x = V.x;
	y = V.y;
	V.x = 0.0f;
	V.y = 0.0f;
	return *this;
}

Vec2 & Vec2::operator=(const Vec2 & V)
{
	x = V.x;
	y = V.y;
	return *this;
}

float Vec2::Dot(const Vec2 & V) const
{
	return ((x * x) + (y * y));
}

float Vec2::Length()const
{
	return sqrt(Dot(*this));
}

Vec2 Vec2::Slope() const
{
	Vec2 temp;
	if (y == 0.0f)
	{
		temp = *this / x;
	}
	else
	{
		temp = *this / y;
	}
	return temp;
}
