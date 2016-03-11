#include "Boss1.h"
#include "StateCore.h"

Boss1::Boss1(StateCore &core)
	:
	Enemy(core),
	BState(Attack),
	FireRate(0.5f),
	RegenRate(3.0f),
	LaunchTimer(0),
	RegenTimer(0),
	mCount(0),
	NMissiles(10),
	shield(core)
{
	health = 10;
	shield.ReduceHealth(-1.0f);
	radius = 16;
	shield.SetRadius(40);
	scoreValue = 1000;
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

void Boss1::DecideState()
{
	readyToFire = false;
	if (mCount < NMissiles)
	{
		BState = Attack;

		if (LaunchTimer < FireRate)
		{
			BState = Idle;
		}
	}
	else
	{
		if (RegenTimer < RegenRate)
		{
			BState = RegenMissiles;
		}
		else
		{
			mCount %= NMissiles;
		}
	}
}

void Boss1::Do()
{
	DecideState();

	switch (BState)
	{
	case Attack:
		readyToFire = true;
		LaunchTimer = 0;
		Fire();
		mCount++;
		RegenTimer = 0;
		break;
	case RegenMissiles:
		UpdateTime(&RegenTimer);
		break;
	case Idle:
		UpdateTime(&LaunchTimer);
		break;
	}
}

//bool Boss1::MissileExploded(unsigned int i)
//{
//	
//	return Missiles[ i ].HasExploded();
//}

//int Boss1::GetMissileRad()
//{
//	return Missiles[0].GetMIR();
//}

void Boss1::Fire()
{
	// needs player position

}
