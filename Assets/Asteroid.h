#pragma once

#include <time.h>

#include "Entity.h"

class Asteroid : public Entity
{
public:
	Asteroid(StateCore &core);

	void Do(  );
	void Draw(const Vec2 &camOffset) const;
	virtual void Init( int x,  int y) = 0;
	int GetAngle() const;
	void DoCollision(Entity *ent);
protected:
	void Move();

protected:
	int angle;
	bool wasHitByPlayerAmmo;
};

class BigAstro : public Asteroid
{
public:
	BigAstro(StateCore &core);
	~BigAstro();

	void Init( int x, int y);
};

class SmallAstro : public Asteroid
{
public:
	SmallAstro( StateCore &core);
	~SmallAstro();

	void Init( int X, int Y );
};