#include "Trigger.h"
#include "TimerServer.h"
#include "StateCore.h"
#include "EntityController.h"
#include "StateController.h"

Trigger::Trigger(StateCore &core)
	:
	core(core),
	passed(false),
	ec(std::make_unique<EntityController>(core) )
{
	x = 0;
	y = 0;	
}

void Trigger::Do()
{
}

void Trigger::Clear()
{
	active = false;
}

void Trigger::Set(float X, float Y)
{
	x = X;
	y = Y;
}

bool Trigger::IsAtTrigger()
{	
	return core.level->GetCam()->GetPosition().y <= y;
}

void Trigger::UpdateTimer( float *eventTimer)
{
	TimerClient tc;
	*eventTimer += tc.GetFrameTime();
}

void Trigger::Activate()
{
	active = true;
}

Trigger::~Trigger()
{
}

// Asteroid Trigger
ATrigger::ATrigger(StateCore &core)
	:
	Trigger(core)
{
	passed = false;
	SpawnRate = 0.5f;
	SpawnTimer = 0.0f;
}

void ATrigger::Do()
{	
	if(IsAtTrigger() && !active )
	{
		Activate();
	}
	if(active && !passed)
	{
		UpdateTimer(&SpawnTimer);
		if(SpawnTimer >= SpawnRate)
		{		
			SpawnTimer = 0;
			
			if(!(ec->SpawnAsteroid(x, y) ) )
			{
				Clear();
				passed = true;
			}
		}
	}
}

ATrigger::~ATrigger()
{
}

// Enemy trigger
ETrigger::ETrigger(StateCore &core)
	:
	Trigger(core)
{
	passed = false;
	SpawnRate = 0.5f;
	SpawnTimer = 0.0f;
}

void ETrigger::SetEnemyType( unsigned int i)
{
	enemyType = i;
}

void ETrigger::Do()
{
	if(IsAtTrigger() && !active && !passed)
	{
		Activate();
		core.enemyCount[enemyType-1] = 0;
	}
	if(active && !passed)
	{
		UpdateTimer(&SpawnTimer);
		if(SpawnTimer >= SpawnRate)
		{
			SpawnTimer = 0.0f;
			if(!(ec->SpawnEnemies(core.level->GetCam()->GetPosition().x, core.level->GetCam()->GetPosition().y, enemyType - 1) ) )
			{
				Clear();
				passed = true;
			}
		}
	}
}

ETrigger::~ETrigger()
{
}

// Boss trigger
BTrigger::BTrigger(StateCore &core)
	:
	Trigger(core)
{
	passed = false;
}

void BTrigger::Do()
{
	if(IsAtTrigger() && !active)
	{
		Activate();
	}
	if(active && !passed)
	{
		if(!core.boss)
		{
			ec->SpawnBoss(core.currentLevel);
		}
	}
}

BTrigger::~BTrigger()
{
}

