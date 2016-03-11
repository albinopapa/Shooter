#include "StateCore.h"
#include "EntityController.h"
#include "GameState.h"

StateCore::StateCore(D3DGraphics &gfx, DSound& audio)
		:
	gfx(gfx),
	audio( audio ),
	ic(gfx),
	player(ic, *this),
	level(nullptr),
	boss(nullptr),
	curState(NONE),
	prevState(NONE),
	currentLevel(1),
	gameStarted(false)
{
	for(unsigned int i = 0; i < MAXENEMYTYPES; i++)
	{
		enemyCount[ i ] = 0;
	}		
}

StateCore::~StateCore()
{
	for(unsigned int i = 0; i < ammo.size(); i++)
	{
		delete ammo[ i ];
		ammo[ i ] = nullptr;
	}
	for(unsigned int i = 0; i < asteroid.size(); i++)
	{
		delete asteroid[ i ];
		asteroid[ i ] = nullptr;
	}
	for(unsigned int i = 0; i < enemy.size(); i++)
	{
		delete enemy[ i ];
		enemy[ i ] = nullptr;
	}
	ammo.clear();
	asteroid.clear();
	enemy.clear();
}