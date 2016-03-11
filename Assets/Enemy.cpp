#include "Enemy.h"
#include "StateCore.h"

Enemy::Enemy(StateCore &core)
	:
	Ship(core),
	Nextp(NULL),
	CurPoint(1)
{
	velocity = 180;
	maxSpeed = 3;
	radius = 32;
	health = 0.50f;
	scoreValue = 10;
	damage = 0.20f;
	hitByAmmo = false;
	state = Moving;

}

void Enemy::CalculatePath(const Vec2& ePos, const Vec2& pPos)
{
	slope = (ePos - pPos).Slope();
}

void Enemy::CheckforWayPoint()
{
	Vec2 cam = core.level->GetCam()->GetPosition();
	dist = (*Nextp + cam - pos).Length();

	if (dist < radius)
	{
		if (CurPoint < NPoints - 1)
		{
			CurPoint++;
			Nextp = &Points[CurPoint];
		}
		else
		{
			health = 0;
		}
	}
}

void Enemy::Move()
{
	float nrmlSpeed = CalcSpeed(velocity);
	nrmlSpeed = min(nrmlSpeed, maxSpeed);
	pos += slope * nrmlSpeed;
}

void Enemy::Fire()
{

}

void Enemy::Die()
{
}

void Enemy::Do()
{
	Vec2 cam = core.level->GetCam()->GetPosition();
	switch (state)
	{
	case Moving:
		CalculatePath(pos, *Nextp + cam);
		Move();
		CheckforWayPoint();
		break;
	}
}

void Enemy::SetSpawnPoint(const Vec2 &Pos)
{
	SetPosition(Points[0] + pos);
}

void Enemy::RevealPlayerPosition(const Vec2 &Pos)
{
	playerPos = Pos;
}

Enemy::~Enemy()
{
}
