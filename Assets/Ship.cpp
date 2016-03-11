#include "Ship.h"
#include "StateCore.h"

/*  The Ship class is the parent class and will store all functions that are not
	specific to any one ship and it will store virtual functions that will be
	defined in child classes.  Any data that is ship specific will be listed in
	the child class while common data such as speed will be stored in the parent
	class as protected with a Get function for retrieving the data.*/

/*  Ships can be created by listing the X and Y distance from the center of the ship
	in the constructor of the child class.
	If ships take up 32x32 then center would be x=15, y=15
	List the vertices either in a clockwise or anticlockwise manner, as long as each
	point connects, the DrawShip function in Game will connect the last point in the
	list to the first point. */

Ship::Ship(StateCore &core)
	:
	Entity(core)
{
}

void Ship::AdjustPosition( float camMoveDistance )
{
	pos.y -= camMoveDistance;
}

void Ship::Draw(const Vec2& camOffset)
{
	D3DGraphics::Color color = core.gfx.FILLCOLOR_XRGB(0, 255, 255);
	Vec2 ship_offset(WorldToScreen(pos, scrnSize, camOffset));

	for(unsigned int i = 0; i < numVerts; i++)
	{
		unsigned int j = (i+1) % numVerts;
		
		Vec2 vert_offset_i(ScreenRes(verts[i], scrnSize));
		Vec2 vert_offset_j(ScreenRes(verts[j], scrnSize));
		core.gfx.DrawShip(ship_offset, vert_offset_i, vert_offset_j, color);
	}
}

Ship::~Ship()
{
}

//  ------------End of Ship class-------------


Player::Player(InputController &ic, StateCore &core)
	:
	Ship(core),
	ic(ic),
	shield(core),
	wc(core)
{
	velocity = 350;// Pixels per second
	shield.SetRadius( 40 );
	numVerts = 4;
	radius = 16;
	pos.x = 400;
	pos.y = 300;
	score = 0;
	moveTimer = 0;
	health = 1.0f;
	damage = 0.20f;
	lives = 3;
	energy = 0;
	verts = new Vec2[numVerts];

	// Point 1
	verts[0].x = 0;
	verts[0].y = -15;

	// Point 2
	verts[1].x = -10;
	verts[1].y = 15;

	// Point 3
	verts[2].x = 0;
	verts[2].y = 10;

	// Point 4
	verts[3].x = 10;
	verts[3].y = 15;
}

void Player::Fire(const Vec2& pos)
{
	wc.Fire(pos);	
}

Shield &Player::GetShield()
{
	return shield;
}

void Player::Move( )
{
	float scrnX = 0;
	float scrnY = 0;
	ic.GetAxes(&scrnX, &scrnY);
	if(scrnX != 0)
	{
		scrnX > 0 ? pos.x += min(5, CalcSpeed(velocity)*scrnX) : pos.x += max(-5, CalcSpeed(velocity)*scrnX);
	}
	if(scrnY != 0)
	{
		scrnY > 0 ? pos.y += min(5, CalcSpeed(velocity)* scrnY) : pos.y += max(-5, CalcSpeed(velocity)*scrnY);
	}
	AdjustPosition( core.level->GetCam()->GetDistance() );
	Clip(*core.level->GetCam());
}

void Player::DoCollision( Entity *ent )
{
	if(shield.GetHealth() > 0.0f)
	{
		shield.ReduceHealth(ent->GetDamage() );
	}
	else
	{
		ReduceHealth(ent->GetDamage() );
	}
}

void Player::Die()
{
	if(lives > 0)
	{
		shield.ReduceHealth( -1.0f);
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
	if(health <= 0.0f)
	{
		state = Dead;
	}
	else
	{
		state = Moving;
		if(ic.Firing() )
		{
 			state = Firing;			
		}
		else
		{
			wc.GetWeapon(4)->SetFireState( false );
		}
	}
}

WeaponController *Player::GetWC()
{
	return &wc;
}

void Player::Do()
{
	DecideState(  );
	wc.RechargeEnergy();
	switch(state)
	{
	case Firing:
		// No break; on purpose.  When player fires ship should also move same frame.
		Fire(pos);	
	case Moving:
		Move();
		shield.SetPosition( pos );
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

	if(pos.x <=  left + radius )
	{
		pos.x = left + radius;
	}
	if(pos.y <= top + radius )
	{
		pos.y = top + radius;
	}
	if(pos.x > right - radius)
	{
		pos.x = right - radius;
	}
	if(pos.y > bottom - radius)
	{
		pos.y = bottom - radius;
	}
}

Player::~Player()
{

}

//  End of Player Ship Class

Enemy::Enemy(StateCore &core)
	:
	Ship(core),
	Nextp( NULL ),
	CurPoint( 1 )
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

void Enemy::CheckforWayPoint( )
{	
	Vec2 cam = core.level->GetCam()->GetPosition();
	dist = (*Nextp + cam - pos).Length();

	if( dist < radius )
	{
		if( CurPoint < NPoints - 1 )
		{
			CurPoint++;
			Nextp = &Points[ CurPoint ];
		}
		else
		{
			health = 0;
		}
	}
}

void Enemy::Move()
{
	float nrmlSpeed = CalcSpeed( velocity);
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
	switch(state)
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
	SetPosition( Points[0] + pos );
}

void Enemy::RevealPlayerPosition(const Vec2 &Pos)
{
	playerPos = Pos;
}

Enemy::~Enemy()
{
}
// End of Enemy Base Class

Enemy1::Enemy1(StateCore &core)
	:
	Enemy(core)
{
	maxSpeed = 3.0f;
	NPoints = 3;
	Points = new Vec2[ NPoints ];
	Points[ 0 ].x = 0;										//	*				*
	Points[ 0 ].y = 0;										//
	Points[ 1 ].x = (float)core.gfx.GetWidth();				//
	Points[ 1 ].y = 0;										//
	Points[ 2 ].x = (float)(core.gfx.GetWidth() / 2);		//
	Points[ 2 ].y = (float)(core.gfx.GetHeight() + radius);	//			*
	Nextp = &Points[ 1 ];
	slope.x = 0;
	slope.y = 0;
	numVerts = 4;
	verts = new Vec2[numVerts];
	// Point 1
	verts[0] = Vec2(0.0f, -15.0f);
	// Point 2
	verts[1] = Vec2(-10.0f, 0.0f);
	// Point 3
	verts[2] = Vec2(0.0f, 15.0f);
	// Point 4
	verts[3] = Vec2(10.0f, 0.0f);
}

Enemy1::~Enemy1()
{
	delete [] verts;
	delete [] Points;
}

Enemy2::Enemy2(StateCore &core)
	:
	Enemy(core)
{
	maxSpeed = 3;
	NPoints = 3;
	Points = new Vec2[ NPoints ];
	pos.x = 0;
	pos.y = 0;
	Points[ 0 ].x = 0.0f;									//	*				*
	Points[ 0 ].y = 0.0f;									//	 
	Points[ 1 ].x = (float)core.gfx.GetWidth() / 2.0f;			//	 	
	Points[ 1 ].y = (float)core.gfx.GetHeight();				//	 
	Points[ 2 ].x = (float)core.gfx.GetWidth() + (radius * 2.0f);	//			
	Points[ 2 ].y = 0.0f - (radius * 2.0f);					//			*
	Nextp = &Points[ 1 ];
	slope.x = 0.0f;
	slope.y = 0.0f;

	numVerts = 4;
	verts = new Vec2[numVerts];
	// Point 1
	verts[0] = Vec2(-15.0f, -15.f);
	// Point 2
	verts[1] = Vec2(15.0f, -15.0f);
	// Point 3
	verts[2] = Vec2(15.0f, 15.0f);
	// Point 4
	verts[3] = Vec2(-15.0f, 15.0f);
}

Enemy2::~Enemy2()
{
	delete [] verts;
	delete [] Points;
}

Enemy3::Enemy3(StateCore &core)
	:
	Enemy(core)
{
	maxSpeed = 3;
	NPoints = 2;
	Points = new Vec2[NPoints];
	pos = { 0.0f, 0.0f };
	Points[0] = { 0.0f, 0.0f };
	Points[1] = {
		core.gfx.GetWidth() + (radius * 2.0f),
		core.gfx.GetHeight() + (radius * 2.0f)
	};
	slope = { 0.0f, 0.0f };

	Nextp = &Points[1];

	numVerts = 8;
	verts = new Vec2[numVerts];
	// Point 1
	verts[0] = { -7.0f, -15.0f };
	// Point 2
	verts[1] = { 7.0f, -15.0f };
	// Point 3
	verts[2] = { 16.0f, -7.0f };
	// Point 4
	verts[3] = { 16.0f, 7.0f };
	// Point 5
	verts[4] = { 7.0f, 16.0f };
	//Point 6
	verts[5] = { -7.0f, 16.0f };
	//Point 7
	verts[6] = { -15.0f, 7.0f };
	//Point 8
	verts[7] = { -15.0f, -7.0f };
}

Enemy3::~Enemy3()
{
	delete [] verts;
	delete [] Points;
}

Enemy4::Enemy4(StateCore &core)
	:
	Enemy(core)
{
	int center = core.gfx.GetWidth() / 2;
	int xGap = 50;
	int yGap = 100;
	maxSpeed = 3;
	NPoints = 7;
	Points = new Vec2[ NPoints ];
	for(unsigned int i = 0; i < NPoints; i++)
	{
		xGap *= -1;
		Points[i] = 
		{ 
			static_cast<float>(center + xGap), 
			static_cast<float>(yGap * i) 
		};
	}
	Nextp = &Points[ 1 ];
	slope = { 0.0f, 0.0f };

	numVerts = 7;
	verts = new Vec2[numVerts];
	// Point 1
	verts[0] = { -5.0f, -15.0f };
	// Point 2
	verts[1] = { 0.0f, -5.0f };
	// Point 3
	verts[2] = { -11.0f, -5.0f };
	// Point 4
	verts[3] = { 0.0f, 16.0f };
	// Point 5
	verts[4] = { 11.0f, -5.0f };
	// Point 6
	verts[5] = verts[1];
	// Point 7
	verts[7] = { 5.0f, -15.0f };
}

Enemy4::~Enemy4()
{
	delete [] verts;
	delete [] Points;
}

Enemy5::Enemy5(StateCore &core)
	:
	Enemy(core)
{
	maxSpeed = 3;
	NPoints = 5;
	Points = new Vec2[ NPoints ];
	
	float scrWidth = static_cast<float>(core.gfx.GetWidth());
	float scrHeight = static_cast<float>(core.gfx.GetHeight());

	Points[0] = { scrWidth * 0.5f, 0.0f };
	Points[1] = { 0.0f, scrHeight * 0.5f };
	Points[2] = { scrWidth * 0.5f, scrHeight };
	Points[3] = { scrWidth, scrHeight * 0.5f };
	Points[4] = { (scrWidth * 0.5f) + (radius * 2.0f), -(radius * 2.0f) };

	Nextp = &Points[ 1 ];
	slope = { 0.0f, 0.0f };

	numVerts = 10;
	verts = new Vec2[numVerts];
	// Point 1
	verts[0] = { -9.0f, -15.0f };
	// Point 2
	verts[1] = { -6.0f, -3.0f };
	// Point 3
	verts[2] = { -15.0f, 4.0f };
	// Point 4
	verts[3] = { -3.0f, 4.0f };
	// Point 5
	verts[4] = { 0.0f, 16.0f };
	// Point 6
	verts[5] = { 4.0f, 4.0f };
	// Point 7
	verts[6] = { 16.0f, 4.0f };
	// Point 8
	verts[7] = { 7.0f, -3.0f };
	// Point 9
	verts[8] = { 10.0f, -15.0f };
	// Point 10
	verts[9] = { 0.0f, -8.0f };
}

Enemy5::~Enemy5()
{
	delete [] verts;
	delete [] Points;
}

/*
Still could use as many different types as possible to give the game some variety.  
Different flight paths different look, different stats, options of firing.
*/
// End of Enemy Derived classes

Boss1::Boss1(StateCore &core)
	:
	Enemy(core),
	BState( Attack ),
	FireRate(0.5f),
	RegenRate(3.0f),
	LaunchTimer(0),
	RegenTimer(0),
	mCount(0),
	NMissiles(10),
	shield(core)
{
	health = 10;
	shield.ReduceHealth( -1.0f );
	radius = 16;
	shield.SetRadius( 40);
	scoreValue = 1000;
	numVerts = 4;
	verts = new Vec2[numVerts];
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
	if(mCount < NMissiles )
	{
		BState = Attack;

		if(LaunchTimer < FireRate)
		{
			BState = Idle;
		}
	}
	else
	{
		if(RegenTimer < RegenRate)
		{
			BState = RegenMissiles;
		}
		else
		{
			mCount %= NMissiles;
		}
	}
}

void Boss1::Do( )
{
	DecideState();

	switch( BState )
	{
	case Attack:
		readyToFire = true;
		LaunchTimer = 0;
		Fire();
		mCount++;
		RegenTimer = 0;
		break;
	case RegenMissiles:
		UpdateTime( &RegenTimer );
		break;
	case Idle:
		UpdateTime( &LaunchTimer );
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

// Boss2 class
Boss2::Boss2(StateCore &core )
	: 
	Enemy(core),
	ramCooldown( 3 ),
	bossState( None )
{
	velocity = 800;
	maxSpeed = 13;
	NPoints = 1;
	Points = new Vec2[ NPoints ];
	Points[0] = { 0.0f, 0.0f };
	Nextp = &Points[ 0 ];

	numVerts = 4;
	verts = new Vec2[numVerts];
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
	switch(bossState)
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
	if( ramCooldownTime >= ramCooldown * 1000 )
	{
		ramCooldownTime = 0;
		bossState = Raming;
		Points[0] = pos;
	}
}

void Boss2::CheckforWayPoint( )
{
	dist = Vec2(rise, run).Length();
	if( dist < 15 )
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
