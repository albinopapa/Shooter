#include "Asteroid.h"
#include "StateCore.h"

Asteroid::Asteroid(StateCore &core )
	:
	Entity( core )
{
}

void Asteroid::Move()
{	
	float speed = CalcSpeed( velocity);
	Vec2 slope = CalcAngleToSlope( angle );
	pos = slope * speed;	
}

void Asteroid::Do()
{
	Move();
}

void Asteroid::DoCollision( Entity *ent )
{
	ReduceHealth(ent->GetDamage());
	angle = 180 - (angle - 180);
	ent->ReduceHealth(damage);
	ent->SetHitByAmmo(false);
}

void Asteroid::Draw(const Vec2 &camOffset ) const
{	
	core.gfx.DrawDisc(pos.x, pos.y - camOffset.y, radius, core.gfx.FILLCOLOR_XRGB(255, 128, 0));
}

int Asteroid::GetAngle() const
{
	return angle;
}

// Big Asteroid class
BigAstro::BigAstro( StateCore &core)
	:
	Asteroid(core)
{
}

BigAstro::~BigAstro()
{
}

void BigAstro::Init( int X,  int Y)
{	
	health = 10;
	radius = 50;
	scoreValue = 50;	
	pos = Vec2(X, Y);
	angle = (rand() % 180) + 90;
	damage = 3;
	wasHitByPlayerAmmo = false;	
}

// Small Asteroid class
SmallAstro::SmallAstro( StateCore &core)
	:
	Asteroid(core )
{
	Init(pos.x, pos.y);
}

SmallAstro::~SmallAstro()
{
}

void SmallAstro::Init( int X,  int Y)
{	
	health = 5;
	radius = 25;
	scoreValue = 25;	
	pos = Vec2(X, Y);
	angle = (rand() % 180) + 90;
	damage = 1.5;
	wasHitByPlayerAmmo = false;	
}

