#include "Shield.h"
#include "TimerServer.h"
#include "StateCore.h"

Shield::Shield(StateCore &core)
	:
	Entity(core),
	rechargeRate(1.0f),
	rechargeTimer(0),
	respawnMax(1.0f)
{
	health = 1.0f;
	damage = 0;
}

void Shield::SetRadius( float rad )
{
	radius = rad;
}

void Shield::Do( )
{
	RechargeShield();
}

void Shield::RechargeShield()
{
	if(health < 1.0f)
	{
		UpdateTime( &rechargeTimer);	
		if(rechargeTimer <= rechargeRate)
		{
			rechargeTimer = 0;
			health += 0.01f;
		}
	}
}

void Shield::Draw(const Vec2& camOffset)
{	
	if(health > 0.0f)
	{
		core.gfx.DrawShield(WorldToScreen( pos, scrnSize, camOffset ), radius, health, scaleFactor.y);
	}
}

