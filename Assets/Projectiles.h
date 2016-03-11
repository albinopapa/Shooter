#pragma once

#include "Entity.h"
#include "Vec2.h"

class Projectile : public Entity
{
public:

	Projectile(const Vec2 &Position, const Vec2 &Slope, StateCore &Core );
	virtual ~Projectile();

	virtual void Do();
	virtual void Draw(const Vec2 &camOffset);
	void DoCollision(Entity *ent);
	void Move();
	bool leavesScreen();
	bool HasTimeToLive();
	void SetLaunchPosition(const Vec2 &Position, const Vec2 &slope );
	void SetCamPosition(const Vec2 &CamPosition);
protected:
	float angle;
	float TTL;
	Vec2 slope;
	Vec2 cam;
	float liveTime, fireTime;			// Pass to UpdateTime(&liveTime) or use &fireTime  
										// When liveTime or fireTime reaches Firerate or TTL set them to 0.0
};

class MGun : public Projectile
{
public:
	MGun(const Vec2 &Position, const Vec2 &Slope, StateCore &Core);
	void Draw(Vec2 &camOffset);
};

class EnergyBall : public Projectile
{
public:
	EnergyBall(const Vec2 &Position, const Vec2 &Slope, StateCore &Core);
	void Draw(Vec2 &camOffset);
private:
	TimerClient tc;
	float radIncRate;
	virtual void Do();
	void ModifyRadius( float amount );

};


class Missile : public Projectile
{
public:
	enum State
	{
		None,
		Acquire,
		Launched,
		Exploded,
		Reset
	};
	Missile(const Vec2 &Position, const Vec2 &Slope, StateCore &Core);
	void DoState( int targetX,  int targetY, bool canFire);
	void SetMissileState( State state );
	int GetMIR() const;
	bool HasExploded();
private:
	void DecideState();
	void DestroyMissiles();
	void Explode();
	void ResetMissile();
	void LaunchMissiles();
	void AcquireTarget( int targetX,  int targetY);
	void Draw(Vec2 &camOffset);

	State mState;
	float LaunchTimer, ExplodeTimer, frameTimer;
	const float explodeDuration;
	const float MIR;									//Missile Impact Radius
	bool Fired, targetAcquired;
};

