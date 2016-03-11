#pragma once
#include "Projectiles.h"

class Laser : public Projectile
{
public:
	Laser();
	Laser(const Vec2 &Position, const Vec2 &Slope, StateCore &Core);
	void Draw(Vec2 &camOffset);
	bool HasCollided(Entity *src2);
	void Do();
	void UpdateEndPoint( Vec2 &c );
	void SetLaunchPosition(const Vec2 &Position, const Vec2 &Slope);
	bool CompareTL_BR();
	bool CompareBL_BR();
	bool CompareBL_TR();
private:
	Vec2 Cam;
	Vec2 CEndPoint;
	Vec2 EndPoint1;
	Vec2 EndPoint2;
};
