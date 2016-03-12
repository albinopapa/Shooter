#include "EntityController.h"
#include <algorithm>
#include "ScoreController.h"
#include "StateCore.h"

EntityController::EntityController(StateCore &core)
	:
	core(core)
{
}

EntityController::~EntityController()
{
}

// Collision function
void EntityController::CheckCollision(Entity *src1, Entity *src2)
{
	if (src1->HasCollided(src2))
	{
		src1->DoCollision(src2);
		src2->DoCollision(src1);
	}
}


// Entity update functions

void EntityController::Update(Entity *ent)
{
	ent->Do();
}

bool EntityController::SpawnEnemies(int X, int Y, unsigned int enemyType)
{
	bool isSpawned = false;

	if (core.enemyCount[enemyType] < 10)
	{
		switch (enemyType)
		{
		case 0:
			eHelper.e = std::make_unique<Enemy1>(core);
			break;
		case 1:
			eHelper.e = std::make_unique< Enemy2>(core);
			break;
		case 2:
			eHelper.e = std::make_unique< Enemy3>(core);
			break;
		case 3:
			eHelper.e = std::make_unique< Enemy4>(core);
			break;
		case 4:
			eHelper.e = std::make_unique< Enemy5>(core);
			break;
		}
	}

	if (eHelper.e)
	{
		core.enemyCount[enemyType] += 1;
		eHelper.e->SetSpawnPoint({ static_cast<float>(X), static_cast<float>(Y) });
		core.enemy.push_back(std::move(eHelper.e));
		isSpawned = true;
	}
	return isSpawned;
}

bool EntityController::SpawnProjectile(std::unique_ptr<Projectile> &pType)
{
	core.ammo.push_back(std::move(pType));
	return true;
}

void EntityController::CreateBigAsteroid(std::unique_ptr<Asteroid> &ast)
{
	core.asteroid.push_back(std::move(ast));
}

void EntityController::CreateSmallAsteroid(std::unique_ptr<Asteroid> &ast)
{
	/*int index = astros.size() - 1;
	float X = astros[ index ]->GetX();
	float Y = astros[ index ]->GetY();
	unsigned int Radius = astros[ NAst - 1]->GetRadius();

	index++;
	astros.push_back( asteroid );
	astros[ index ]->SetPosition( X - Radius, Y - Radius );
	index++;
	astros.push_back( asteroid );
	astros[ index ]->SetPosition( X + Radius, Y + Radius );*/
}

bool EntityController::SpawnAsteroid(int X, int Y)
{
	bool spawned = false;
	if (core.asteroid.size() < 10)
	{
		std::unique_ptr<Asteroid> ast(std::make_unique<BigAstro>(core));
		ast->Init(X, Y);
		CreateBigAsteroid(ast);
		spawned = true;
	}
	return spawned;
}

bool EntityController::SpawnBoss(unsigned int currentLevel)
{
	bool isSpawned = false;
	switch (currentLevel)
	{
	case 1:
		core.boss = std::make_unique<Boss1>(core);
		break;
	case 2:
		core.boss = std::make_unique<Boss2>(core);
		break;
		/*
		case 3:
		core.boss = new Boss3(core);
		break;
		case 4:
		core.boss = new Boss4(core);
		break;
		case 5:
		core.boss = new Boss5(core);
		break;
		case 6:
		core.boss = new Boss6(core);
		break;
		case 7:
		core.boss = new Boss7(core);
		break;
		case 8:
		core.boss = new Boss8(core);
		break;
		*/
	}
	if (core.boss)
	{
		isSpawned = true;
	}
	return isSpawned;
}

bool EntityController::IsDead(Entity *ent)
{
	// Future to do's: 
	// erase() in vector is supposedly inefficient, std::replace_if supposedly 
	// better option
	// std::replace_if(vector.begin(), vector.end(), boolReturningFunction(), newValue);
	// boolReturningFunction() <- could be something that checks if(health <= 0) return true;
	// newValue would be something like vector.end() since the order of entities shouldn't matter
	//
	// Could also impliment a custom method that does the same but after moving vector.end() to 
	// new index spot, would call vector.pop_back()
	/*for( unsigned int index = 0; index < core.asteroid.size(); index++ )
	{
	float radius = core.asteroid[index]->GetRadius();
	float health = core.asteroid[index]->GetHealth();

	if( health <= 0  )
	{
	if (core.asteroid[index]->GotHitByAmmo())
	{
	core.cScore.Add(core.asteroid[index]->GetValue());
	}
	if( radius > 25.0f)
	{
	CreateSmallAsteroid(new SmallAstro(core));
	}
	core.asteroid.erase(core.asteroid.begin() + index);
	}
	}
	for( unsigned int i = 0; i < core.enemy.size(); i++)
	{
	if(core.enemy[ i ]->GetHealth() <= 0.0f)
	{
	if( core.enemy[ i ]->GotHitByAmmo() )
	{
	core.cScore.Add( core.enemy[ i ]->GetValue() );
	}
	core.enemy.erase(core.enemy.begin() + i);
	}
	}
	*/
	return ent->GetHealth() <= 0.0f;
}
