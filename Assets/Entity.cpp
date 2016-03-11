#include "Entity.h"
#include "TimerServer.h"
#include "StateCore.h"

Entity::Entity(StateCore &core)
	:
	core(core)
{
	scrnSize.x = core.gfx.GetWidth();
	scrnSize.y = core.gfx.GetHeight();
	scaleFactor = CalcScreenScale( scrnSize );
}

void Entity::Do()
{
}

float Entity::GetDamage( )
{	
	return damage;
}

unsigned int Entity::GetValue()
{
	return scoreValue;
}

bool Entity::CheckIsAlive( )
{
	return health > 0;
}

void Entity::SetPosition(const Vec2 &Pos )
{
	pos = Pos;
}

void Entity::UpdateTime( float *eventTimer)
{
	TimerClient tc;
	*eventTimer += tc.GetFrameTime();	
}

bool Entity::HasCollided(Entity *src2)
{
	const Vec2 dist = (src2->GetC() - pos);
	float distSqr = dist.Dot(dist);
	float rSqrSum = (radius * radius) + (src2->GetRadius() * src2->GetRadius());
	return distSqr <= rSqrSum;
}

float Entity::GetHealth()
{
	return health;
}

void Entity::ReduceHealth( float health )
{
	this->health -= health;
}

void Entity::SetHitByAmmo( bool wasHit )
{
	hitByAmmo = wasHit;
}

bool Entity::GotHitByAmmo()
{
	return hitByAmmo;
}

float Entity::GetX() const
{
	return pos.x;
}

float Entity::GetY() const
{
	return pos.y;
} 

Vec2 Entity::GetC() const
{
	return pos;
}

float Entity::GetRadius() const
{
	return radius;
}

Entity::~Entity()
{}
