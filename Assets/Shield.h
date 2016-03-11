#pragma once

#include "Entity.h"

class Shield : public Entity
{
public:
	Shield(StateCore &core);
	void Do();
	void SetRadius( float rad );
	void Draw(const Vec2& camOffset);
private:
	void RechargeShield();

private:
	const float respawnMax;
	float rechargeTimer;
	const unsigned int rechargeRate;
};