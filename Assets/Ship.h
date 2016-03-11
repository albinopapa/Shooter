#pragma once

#include <vector>

#include "InputController.h"
#include "Shield.h"
#include "WeaponController.h"
#include "Vec2.h"

class Ship : public Entity
{
public:
	Ship();
	Ship(StateCore &core);
	virtual ~Ship();

	enum ShipState
	{
		Moving,
		Firing,
		Dead
	};

	void Do() = 0;
	void AdjustPosition( float camMoveDistance );
	void Draw(const Vec2& camOffset);
protected:
	unsigned int numVerts;
	Vec2* verts;

};

class Player: public Ship
{
public:
	Player( InputController &ic, StateCore &core);
	~Player();
	void Do();
	void DoCollision(Entity *ent);
	WeaponController *GetWC();
	Shield &GetShield();
	void Clip(Camera &cam );
private:
	void Move();
	void Fire(const Vec2& pos);
	void Die();
	void DecideState( );

private:
	WeaponController wc;
	unsigned int score;
	int lives;
	ShipState state;
	InputController &ic;
	Shield shield;
	unsigned int energy;

};

class Enemy: public Ship
{
public:
	Enemy( StateCore &core);
	virtual ~Enemy();
	void Move();
	void Fire();
	void Die();
	void Do();
	void RevealPlayerPosition(const Vec2& pPos );
	void SetSpawnPoint(const Vec2& pos );
	virtual void CheckforWayPoint();
	void CalculatePath(const Vec2& ePos, const Vec2& pPos);

protected:
	unsigned int NPoints;
	unsigned int CurPoint;
	float rise, run, dist;
	Vec2 slope;
	Vec2* Points;
	Vec2* Nextp;
	Vec2 playerPos;
	ShipState state;
	float maxSpeed;
};

class Enemy1: public Enemy
{
public:
	Enemy1(StateCore &core);
	~Enemy1();
};

class Enemy2: public Enemy
{
public:
	Enemy2(StateCore &core);
	~Enemy2();
};

class Enemy3: public Enemy
{
public:
	Enemy3(StateCore &core);
	~Enemy3();
};

class Enemy4: public Enemy
{
public:
	Enemy4(StateCore &core);
	~Enemy4();
};

class Enemy5: public Enemy
{
public:
	Enemy5(StateCore &core);
	~Enemy5();
};

class Boss1: public Enemy
{
public:
	enum State 
	{
		Attack,
		RegenMissiles,
		Idle
	};
	Boss1(StateCore &core);
	void Do();

	// int GetMissileRad();
	// bool MissileExploded( unsigned int i );
	float GetMissileDamage();

private:
	void Fire();
	void DecideState();
	void MissilesRegen();
	float temp;						// stores progressive time between calls for event handling

	float LaunchTimer, RegenTimer;	// Event timer triggers

	State BState;
	int pX, pY;						// playerX and playerY to pass to Missile class
	const float FireRate, RegenRate;
	unsigned int NMissiles;			// Number Of Missiles
	unsigned int MissilesFired;		// Counter for firing missiles and event handling
	unsigned int mCount;
	bool readyToFire;
	Shield shield;
};

class Boss2: public Enemy
{
public:
	enum bossState
	{
		Raming,
		None
	};
	Boss2(StateCore &core);
	void Do();
	float GetPlayerX();
	float GetPlayerY();
	void SetPlayerPos( int X, int Y );
private:
	void CheckforWayPoint();
	void TransitionState();
	bossState bossState;

	const float ramCooldown;
	float ramCooldownTime;
};
