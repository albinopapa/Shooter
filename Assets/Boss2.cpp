#include "Boss2.h"
#include "StateCore.h"

Boss2::Boss2(StateCore &core)
	:
	Enemy(core),
	ramCooldown(3),
	bossState(None)
{
	velocity = 800;
	maxSpeed = 13;
	NPoints = 1;
	Points = std::make_unique<Vec2[]>(NPoints);
	Points[0] = { 0.0f, 0.0f };
	Nextp = &Points[0];

	numVerts = 4;
	verts = std::make_unique<Vec2[]>(numVerts);
	// Point 1
	verts[0] = { 0.0f, -15.0f };
	// Point 2
	verts[1] = { -10.0f, 0.0f };
	// Point 3
	verts[2] = { 0.0f, 15.0f };
	// Point 4
	verts[3] = { 10.0f, 0.0f };
}

void Boss2::Do()
{
	switch (bossState)
	{
	case Raming:
		Move();
		CheckforWayPoint();
		break;
	case None:
		TransitionState();
		break;
	}
}

void Boss2::TransitionState()
{
	if (ramCooldownTime >= ramCooldown * 1000)
	{
		ramCooldownTime = 0;
		bossState = Raming;
		Points[0] = pos;
	}
}

void Boss2::CheckforWayPoint()
{
	dist = Vec2(rise, run).Length();
	if (dist < 15)
	{
		bossState = None;
		ramCooldownTime = 0;
	}

}

float Boss2::GetPlayerX()
{
	return pos.x;
}

float Boss2::GetPlayerY()
{
	return pos.y;
}
