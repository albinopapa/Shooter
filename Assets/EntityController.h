#pragma once

#include "StandardLibraryIncludes.h"
#include "Asteroid.h"
#include "ShipIncludes.h"
#include "Camera.h"

class StateCore;

class EntityController
{
struct eSpawnHelper
{
	unsigned int eMax;
	std::unique_ptr<Enemy> e;
};

public:
	EntityController(StateCore &core);
	~EntityController();
	bool IsDead(Entity *ent);
	bool SpawnAsteroid(int X, int Y);
	bool SpawnProjectile(std::unique_ptr<Projectile> &pType);
	bool SpawnEnemies(int X, int Y, unsigned int enemyType );
	bool SpawnBoss( unsigned int currentLevel);

	// Collision function
	void CheckCollision(Entity *src1, Entity *src);

	// Entity update Methods
	void Update(Entity *ent);

private:
	// Private Methods
	
	// Factory functions
	void CreateBigAsteroid(std::unique_ptr<Asteroid> &ast);
	void CreateSmallAsteroid(std::unique_ptr<Asteroid> &ast);
	
	// Private members
private:
	StateCore &core;
	eSpawnHelper eHelper;
};