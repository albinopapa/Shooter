#pragma once
#include "Enemy.h"

class Boss2 : public Enemy
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
	void SetPlayerPos(int X, int Y);
private:
	void CheckforWayPoint();
	void TransitionState();
	bossState bossState;

	const float ramCooldown;
	float ramCooldownTime;
};
