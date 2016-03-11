#include "Player.h"
#include "StateCore.h"

Player::Player(InputController &ic, StateCore &core)
	:
	Ship(core),
	ic(ic),
	shield(core),
	wc(core)
{
	velocity = 350;// Pixels per second
	shield.SetRadius(40);
	numVerts = 4;
	radius = 16;
	pos = { 400.0f, 300.0f };
	score = 0;
	moveTimer = 0;
	health = 1.0f;
	damage = 0.20f;
	lives = 3;
	energy = 0;
	verts = std::make_unique<Vec2[]>(numVerts);

	// Point 1
	verts[0] = { 0.0f, -15.0f };
	// Point 2
	verts[1] = { -10.0f, 15.0f };
	// Point 3
	verts[2] = { 0.0f, 10.0f };
	// Point 4
	verts[3] = { 10.0f, 15.0f };
}

void Player::Fire(const Vec2& Pos)
{
	wc.Fire(Pos);
}

Shield &Player::GetShield()
{
	return shield;
}

void Player::Move()
{
	Vec2 scrn(ic.GetAxes());
	if (scrn.x != 0)
	{
		scrn.x > 0 ? pos.x += min(5, CalcSpeed(velocity)*scrn.x) : pos.x += max(-5, CalcSpeed(velocity)*scrn.x);
	}
	if (scrn.y != 0)
	{
		scrn.y > 0 ? pos.y += min(5, CalcSpeed(velocity)* scrn.y) : pos.y += max(-5, CalcSpeed(velocity)*scrn.y);
	}
	AdjustPosition(core.level->GetCam()->GetDistance());
	Clip(*core.level->GetCam());
}

void Player::DoCollision(Entity *ent)
{
	if (shield.GetHealth() > 0.0f)
	{
		shield.ReduceHealth(ent->GetDamage());
	}
	else
	{
		ReduceHealth(ent->GetDamage());
	}
}

void Player::Die()
{
	if (lives > 0)
	{
		shield.ReduceHealth(-1.0f);
		health = 1.0f;
		lives--;
		SetPosition(scrnSize / 2);
	}
	else
	{
		core.curState = core.GAMEOVER;
	}
}

void Player::DecideState()
{
	if (health <= 0.0f)
	{
		state = Dead;
	}
	else
	{
		state = Moving;
		if (ic.Firing())
		{
			state = Firing;
		}
		else
		{
			wc.GetWeapon(4)->SetFireState(false);
		}
	}
}

WeaponController *Player::GetWC()
{
	return &wc;
}

void Player::Do()
{
	DecideState();
	wc.RechargeEnergy();
	switch (state)
	{
	case Firing:
		// No break; on purpose.  When player fires ship should also move same frame.
		Fire(pos);
	case Moving:
		Move();
		shield.SetPosition(pos);
		shield.Do();
		break;
	case Dead:
		Die();
		break;
	}

}

void Player::Clip(Camera &cam)
{
	unsigned int left = cam.GetViewport().x1;
	unsigned int top = cam.GetViewport().y1;
	unsigned int right = cam.GetViewport().x2;
	unsigned int bottom = cam.GetViewport().y2;

	if (pos.x <= left + radius)
	{
		pos.x = left + radius;
	}
	if (pos.y <= top + radius)
	{
		pos.y = top + radius;
	}
	if (pos.x > right - radius)
	{
		pos.x = right - radius;
	}
	if (pos.y > bottom - radius)
	{
		pos.y = bottom - radius;
	}
}

Player::~Player()
{

}
