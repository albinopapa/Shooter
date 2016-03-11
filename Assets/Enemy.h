#pragma once
#include "Ship.h"

class Enemy : public Ship
{
public:
	Enemy(StateCore &core);
	virtual ~Enemy();
	void Move();
	void Fire();
	void Die();
	void Do();
	void RevealPlayerPosition(const Vec2& pPos);
	void SetSpawnPoint(const Vec2& pos);
	virtual void CheckforWayPoint();
	void CalculatePath(const Vec2& ePos, const Vec2& pPos);

protected:
	unsigned int NPoints;
	unsigned int CurPoint;
	float rise, run, dist;
	Vec2 slope;
	std::unique_ptr<Vec2[]> Points;
	Vec2* Nextp;
	Vec2 playerPos;
	ShipState state;
	float maxSpeed;
};

