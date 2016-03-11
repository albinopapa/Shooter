#include "Weapon.h"
#include "StateCore.h"
#include "Laser.h"

Weapon::Weapon(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost)
	:
	unlocked(unlock),
	have(have),
	curlevel(level),
	name(name),
	core(core)
{
	fireRate = 2.0f;
	fireTimer = 0.0f;
	EnergyDrain = 0.0f;
	dummyPos.x = 0.0f;
	dummyPos.y = 0.0f;
	dummySlope.x = 0.0f;
	dummySlope.y = 0.0f;
	angle[0] = 360.0f;
	angle[1] = 340.0f;
	angle[2] = 20.0f;
	angle[3] = 320.0f;
	angle[4] = 40.0f;
	
}

Weapon::~Weapon()
{
	if(projectileType)
	{
		delete projectileType;
		projectileType = nullptr;
	}
}

float Weapon::GetEnergyDrain()
{
	return EnergyDrain;
}

void Weapon::SetFireState(bool fire)
{
	isFiring = fire;
}

void Weapon::Fire( Vec2 pos )
{
	UpdateTimer( &fireTimer );
	if(fireTimer >= fireRate )
	{
		fireTimer = 0.0f;
		for(unsigned int i = 0; i <= curlevel; i++)
		{
			Vec2 slope = CalcAngleToSlope(angle[i]);
			projectileType->SetLaunchPosition(pos, slope);
			core.ammo.push_back(new Projectile(*projectileType) );
			
		}
	}
}

void Weapon::UpdateTimer( float *eventTimer )
{
	TimerClient tc;
	*eventTimer += tc.GetFrameTime();
}

ECannon::ECannon(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost)
	:
	Weapon(core, unlock, have, level, name, cost)
{
	EnergyDrain = 2.0f;
	projectileType = new EnergyBall(dummyPos, dummySlope, core);
}

ECannon::~ECannon()
{
}

LaserWeap::LaserWeap(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost)
	:
	Weapon(core, unlock, have, level, name, cost)
{
	EnergyDrain = 3.0;
	fireRate = .025;
	isFiring = false;
}



void LaserWeap::Fire( Vec2 pos )
{	
	if(!isFiring)
	{
		for(unsigned int i = 0; i < curlevel; i++)
		{
			Vec2 slope = CalcAngleToSlope(angle[i]);
			core.ammo.push_back( new Laser(pos, slope, core) );		
		}
	}
}

LaserWeap::~LaserWeap()
{
	if(projectileType)
	{
		delete projectileType;
		projectileType = nullptr;
	}	
}

Blaster::Blaster(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost)
	:
	Weapon(core, unlock, have, level, name, cost)
{
	EnergyDrain = 0;
	projectileType = new Projectile(dummyPos, dummySlope, core);
	fireRate = 0.5f;
}

Blaster::~Blaster()
{
	if(projectileType)
	{
		delete projectileType;
		projectileType = nullptr;
	}
}

MachineGun::MachineGun(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost)
	:
	Weapon(core, unlock, have, level, name, cost)
{
	EnergyDrain = 0;
	projectileType = new MGun(dummyPos, dummySlope, core);
	fireRate = 0.16f;
}

MachineGun::~MachineGun()
{
	if(projectileType)
	{
		delete projectileType;
		projectileType = nullptr;
	}
}

MLauncher::MLauncher(StateCore &core, bool unlock, bool have, unsigned int level, std::string name, unsigned int cost)
	:
	Weapon(core, unlock, have, level, name, cost)
{
	projectileType = new Projectile(dummyPos, dummySlope, core);
	fireRate = 0.5f;
}

MLauncher::~MLauncher()
{
	if(projectileType)
	{
		delete projectileType;
		projectileType = nullptr;
	}
}

