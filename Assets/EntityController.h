#pragma once

#include "Entity.h"
#include <vector>
#include "Asteroid.h"
#include "Ship.h"
#include "Camera.h"

class StateCore;

class EntityController
{
struct eSpawnHelper
{
	unsigned int eMax;
	Enemy *e;
};

public:
	EntityController(StateCore &core);
	~EntityController();
	bool IsDead(Entity *ent);
	bool SpawnAsteroid(int X, int Y);
	bool SpawnProjectile(Projectile *pType);
	bool SpawnEnemies(int X, int Y, unsigned int enemyType );
	bool SpawnBoss( unsigned int currentLevel);

	// Collision function
	void CheckCollision(Entity *src1, Entity *src);

	// Entity update Methods
	void Update(Entity *ent);
	void EraseEntity(Entity *ent);

private:
	// Private Methods
	
	// Factory functions
	void CreateBigAsteroid(Asteroid * ast);
	void CreateSmallAsteroid(Asteroid *ast);
	
	// Private members
private:
	StateCore &core;
	eSpawnHelper eHelper;
};