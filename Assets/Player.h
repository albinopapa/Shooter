#pragma once
#include "Ship.h"
#include "WeaponController.h"

class Player : public Ship
{
public:
	Player(InputController &ic, StateCore &core);
	~Player();
	void Do();
	void DoCollision(Entity *ent);
	WeaponController *GetWC();
	Shield &GetShield();
	void Clip(Camera &cam);
private:
	void Move();
	void Fire(const Vec2& pos);
	void Die();
	void DecideState();

private:
	WeaponController wc;
	unsigned int score;
	int lives;
	ShipState state;
	InputController &ic;
	Shield shield;
	unsigned int energy;

};

