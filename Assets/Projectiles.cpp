#include "Projectiles.h"
#include "StateCore.h"

Projectile::Projectile(const Vec2 &Position, const Vec2 &Slope, 
	StateCore &Core)
	:
	Entity(Core)
{
	health = 0.10f;
	velocity = 600.0f;
	damage = 0.5f;
	radius = 2.0f;
	liveTime = 0.0f;
	SetLaunchPosition(Position, Slope);
}

void Projectile::DoCollision(Entity *ent)
{
	health = 0;
	ent->ReduceHealth( damage );
	ent->SetHitByAmmo( true );
}

void Projectile::Move()
{
	float speed = CalcSpeed( velocity );
	pos.y += speed * slope.y;
	pos.x += speed * slope.x;
}

void Projectile::SetCamPosition(const Vec2 &CamPosition)
{
	cam = CamPosition;
}

void Projectile::SetLaunchPosition(const Vec2 &Position, const Vec2 &Slope)
{
	pos = Position;
	slope = Slope;
}

bool Projectile::leavesScreen()
{
	return pos.x < 0.0f || pos.y < 0.0f || pos.x >= core.gfx.GetWidth() || 
		pos.y >= core.gfx.GetHeight();
}

bool Projectile::HasTimeToLive()
{
	return (liveTime < TTL) || !leavesScreen();
}

void Projectile::Draw(const Vec2 &CamOffset)
{
  	core.gfx.DrawAmmo( GameToScreenVec2( pos, scrnSize, CamOffset), scaleFactor );
}

void Projectile::Do()
{
	UpdateTime( &fireTime );
	UpdateTime( &liveTime );
	Move();	
}

Projectile::~Projectile()
{
}

// Machine Gun class
MGun::MGun(const Vec2 &Position, const Vec2 &Slope, StateCore &Core)
	:
	Projectile(Position, Slope, Core)
{
	radius = 2;
	damage = 0.25f;
}

void MGun::Draw(Vec2 &camOffset)
{
	core.gfx.DrawAmmo( GameToScreenVec2( pos, scrnSize, camOffset ), scaleFactor );
}

// Missile Class Begin

Missile::Missile(const Vec2 &Position, const Vec2 &Slope, StateCore &Core)
	:
	Projectile(Position, Slope, Core),
	MIR( 10.0f ),
	explodeDuration(0.3f)
{
	TTL = 1.0f;
	targetAcquired = false;
	damage = 10.0f;
}

void Missile::DecideState()
{
	if(!targetAcquired)
	{
		if(Fired)
		{
			mState = Acquire;
		}
		else
		{
			mState = None;
		}
	}
	else if(LaunchTimer < TTL * 1000 && targetAcquired)
	{
		mState = Launched;
	}
	else
	{
		if( ExplodeTimer < explodeDuration * 1000)
		{
			mState = Exploded;
		}
		else
		{
			mState = Reset;
		}
	}
}

void Missile::DoState( int tX, int tY, bool canFire )
{
	Fired = canFire;
	DecideState();
	switch(mState)
	{
	case Acquire:
		AcquireTarget(tX, tY);
		break;
	case Launched:
		LaunchMissiles();
		break;
	case Exploded:
		Explode();
		break;
	case Reset:
		ResetMissile();
		break;
	
	}
}

void Missile::AcquireTarget(  int tX,  int tY )
{
	// Randomize X, Y +/- 100 from player position
	pos.x = (float)(rand() % 200 - 100) + tX;
	pos.y = (float)(rand() % 200 - 100) + tY;
	targetAcquired = true;
}

void Missile::LaunchMissiles()
{
	UpdateTime(&LaunchTimer);
}

void Missile::Explode()
{
	UpdateTime(&ExplodeTimer);
}

void Missile::ResetMissile( )
{
	pos.x = 0;
	pos.y = 0;
	ExplodeTimer = 0;
	LaunchTimer = 0;
	targetAcquired = false;
}

bool Missile::HasExploded()
{
	return mState == Exploded;
}

int Missile::GetMIR() const
{
	return MIR;
}

void Missile::Draw(Vec2 &camOffset)
{
	D3DGraphics::Color c;
	switch( mState )
	{
	case Launched:
		c = core.gfx.FILLCOLOR_XRGB(0, 255, 0);
		break;
	case Exploded:
		c = core.gfx.FILLCOLOR_XRGB(255, 0, 0);
		break;
	}
	core.gfx.DrawCircle( GameToScreenVec2( pos, scrnSize, camOffset ), MIR, scaleFactor.y, c);
}

// EnergyBall class
EnergyBall::EnergyBall(const Vec2 &Position, const Vec2 &Slope, StateCore &Core)
	:
	Projectile(Position, Slope, Core)
{
	TTL = 1.0f;
	velocity = 300.0f;
	radIncRate = 5.0f;
	radius = 2.5f;
	health = 1.0f;
	damage = 1.0f;
	SetLaunchPosition(pos, slope);
}

void EnergyBall::Do()
{
	float frametime = 0.0f;
	UpdateTime( &frametime );
	ModifyRadius( radIncRate * frametime );
	Move();
}

void EnergyBall::ModifyRadius( float amount )
{
	radius += amount;
}

void EnergyBall::Draw(Vec2 &camOffset)
{
	core.gfx.DrawDisc( GameToScreenVec2( pos, scrnSize, camOffset ), radius, scaleFactor.y, core.gfx.FILLCOLOR_XRGB( 255,255,255 ) );
}
