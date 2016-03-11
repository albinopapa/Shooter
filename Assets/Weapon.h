#pragma once

#define nWeps 5
#define nRes 3
#define objMax nWeps + nRes			
#define nUpgrades 5					// Number of weapons to unlock

#include "Projectiles.h"
#include <string>

class Weapon
{
	friend class WeaponController;
public:
	Weapon(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost);
	virtual ~Weapon();
	virtual void Fire( Vec2 pos );
	float GetEnergyDrain();
	float GetRechargeRate();
	void SetFireState(bool fire);
protected:
	void UpdateTimer(float *eventType);

	bool unlocked;
	bool have;
	bool isFiring;
	std::string name;
	float angle[ 5 ];
	unsigned int curlevel, nextLevel;
	float fireRate, fireTimer;
	Projectile *projectileType;
	StateCore &core;
	// Weapon attributes
	Vec2 dummyPos;
	Vec2 dummySlope;
	float EnergyDrain;

};

class LaserWeap : public Weapon
{
public:
	LaserWeap(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost);
	~LaserWeap();
	virtual void Fire( Vec2 pos );
};

class MachineGun : public Weapon
{
public:
	MachineGun(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost);
	~MachineGun();
};

class ECannon : public Weapon
{
public:
	ECannon(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost);
	~ECannon();
};

class Blaster : public Weapon
{
public:
	Blaster(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost);
	~Blaster();
};

class MLauncher : public Weapon
{
public:
	MLauncher(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost);	
	~MLauncher();

private:
	int maxMissiles, missileCount;
};