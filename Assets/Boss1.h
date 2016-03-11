#pragma once
#include "Enemy.h"

class Boss1 : public Enemy
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

