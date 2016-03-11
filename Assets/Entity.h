#pragma once
#include "TimerServer.h"
#include "Level.h"

// The purpose of this class is to be the storage of similar traits among the objects in the game
// such as position (X, Y), velocity, health, damage, scoreValue and size(radius)

class StateCore;

class Entity
{
public:
	Entity(StateCore &core);
	~Entity();
	virtual void Do();
	virtual void Move(){};
	virtual void Draw(D3DGraphics &gfx) {};
	virtual void Die() {};
	virtual void DoCollision( Entity *ent ){};
	virtual bool HasCollided(Entity *src2);

	float GetX() const;
	float GetY() const;
	Vec2 GetC() const;
	float GetRadius() const;
	float GetHealth();
	bool GotHitByAmmo();
	float GetDamage();
	unsigned int GetValue();
	
	void SetPosition(const Vec2& pos );
	void ReduceHealth( float health );
	void SetHitByAmmo( bool wasHit );
protected:
	bool CheckIsAlive( );
	void UpdateTime(float *eventTimer);

protected:
	// physical attributes
	StateCore &core;
	float radius, scoreValue;
	Vec2 pos, scaleFactor, scrnSize;
	float health, damage, velocity;
	float moveTimer;
	bool hitByAmmo;
	int maxVelocity;
};
