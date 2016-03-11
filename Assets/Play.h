#pragma once

#include "GameState.h"
#include "EntityController.h"
class Play : public GameState
{
public:
	Play(StateController &sc, StateCore &core);
	void Do()override;

private:  // private functions
	void CheckTriggers();
	void CheckInput();
	void UpdatePlayer();
	void UpdateCamera();
	void UpdateEnemies();
	void UpdateAsteroids();
	void UpdateAmmo();
	void UpdateBoss();

	void CheckCollisions();
	void CleanVectors();

	void BeginFrame()override;
	void ComposeFrame()override;

private:
	EntityController ec;
	Player *player;
	Camera *cam;
	Level *l;

};

