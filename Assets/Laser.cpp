#include "Laser.h"
#include "StateCore.h"

Laser::Laser(const Vec2 &Position, const Vec2 &Slope, StateCore &Core)
	:
	Projectile(Position, Slope, Core)
{
	TimerClient tc;
	health = 0.0f;
	damage = 1.0f;
	radius = 5.0f;
	TTL = core.player.GetWC()->GetEnergy() / core.player.GetWC()->GetWeapon(4)->GetEnergyDrain() * tc.GetFrameTime();
	liveTime = 0.0f;
	CEndPoint.x = 0.0f;
	CEndPoint.y = 0.0f;
	SetLaunchPosition(pos, slope);
}

void Laser::SetLaunchPosition(const Vec2 &Position, const Vec2 &Slope)
{
	pos = Position;
	slope = Slope;
	Cam = core.level->GetCam()->GetPosition();
}

bool Laser::CompareTL_BR()
{
	return false;
}

bool Laser::CompareBL_BR()
{
	return false;
}

bool Laser::CompareBL_TR()
{
	return false;
}

bool Laser::HasCollided(Entity* src2)
{
	// Needs to calculate the slope from the two outer corners of the objects
	// for correct collision.
	float src2Radius = src2->GetRadius();
	const Vec2 src2_tl(src2->GetX() - src2Radius, src2->GetY() - src2Radius);
	const Vec2 src2_tr(src2->GetX() + src2Radius, src2->GetY() - src2Radius);
	const Vec2 src2_bl(src2->GetX() - src2Radius, src2->GetY() + src2Radius);
	const Vec2 src2_br(src2->GetX() + src2Radius, src2->GetY() + src2Radius);

	Vec2 SlopeE[4] =
	{
		(src2_tl - pos).Slope(),
		(src2_tr - pos).Slope(),
		(src2_bl - pos).Slope(),
		(src2_br - pos).Slope()
	};

	Vec2 s1 = (EndPoint1 - pos).Slope();	
	Vec2 s2 = (EndPoint2 - pos).Slope();

	bool collided = false;
	for(unsigned int i = 1; i < 4; i++)
	{
		if( (SlopeE[i - 1].x < s1.x && SlopeE[i].x > s2.x) &&
			(SlopeE[i - 1].y == s1.y && SlopeE[i].y == s2.y) )
		{
			collided = true;
		}
	}
	return collided;
}

void Laser::Draw(Vec2 &camOffset)
{
	Vec2 pos2 = pos;
	pos = GameToScreenVec2( pos2, scrnSize, camOffset );
	Vec2 p2 = CEndPoint;
	p2 = GameToScreenVec2( p2, scrnSize, camOffset );
	core.gfx.DrawLine( pos2, p2, core.gfx.FILLCOLOR_XRGB( 255,255,255 ) );
}

void Laser::UpdateEndPoint( Vec2 &EndPosition )
{
	Cam = core.level->GetCam()->GetPosition();
	Vec2 dist;
	float rBounds = core.gfx.GetWidth() - 1;

	// Get X distance 
	dist.x = slope.x * (pos.y - Cam.y);

	// if slope.x is negative then distX will be negative	
	if(pos.x + dist.x < 0 )
	{
		// Get the amount of pixel allowed to move up
		dist.y = pos.x / slope.x;
		// Get new distX, must be negative
		dist.x = -(slope.x * dist.y);
	}
	else if(pos.x + dist.x >= rBounds + 1 )
	{
		// distY needs to be negative so laser goes up the screen
		dist.x = rBounds - pos.x;
		dist.y = -(dist.x / slope.x);
	}
	else
	{
		dist.y = -(pos.y - Cam.y);
	}

	EndPosition = pos + dist;
}

void Laser::Do()
{
	TimerClient tc;
	if(core.ic.Firing() )
	{
		UpdateTime(&liveTime);
		TTL = core.player.GetWC()->GetEnergy() / (core.player.GetWC()->GetWeapon(4)->GetEnergyDrain() * tc.GetFrameTime());
		pos = core.player.GetC();	
		UpdateEndPoint( EndPoint1 );
		CEndPoint = EndPoint2 = EndPoint1;	
		EndPoint1.x += radius;
		EndPoint2.x -= radius;
	}
	else
	{
		liveTime = TTL;
	}
}


