#pragma once

#define MAXENEMYTYPES 5

#include "StandardLibraryIncludes.h"
// Includes for game objects
#include "Asteroid.h"
#include "Camera.h"
#include "Level.h"
#include "ShipIncludes.h"
#include "ScoreController.h"
#include "Sound.h"

// Inlcudes for framework
#include "InputController.h"

// Forward declarations
class GameState;

// struct that is pretending to be a class
class StateCore 
{
	// Needed more control of what triggers a state transition and what the 
	// previous state is since I haven't figure out a better way than an enum 
	// list here it is.  The list can be extended and then a case can be added 
	// to the switch in StateController.  If anyone knows another way around 
	// it then either let me know or change it.  I've tried adding a prevState
	// pointer, but if it points to state then state gets deleted or changed 
	// so does the prevState pointer.  Can't just create a new state without 
	// knowing which new state to create for prevState.  Thought of using 
	// "new auto(state), that didn't work.  Tried GameState** prevState = &state, 
	// that didn't work.  Thought I could do a void* and create a space in mem 
	// sizeof(state), but then wouldn't know which state to cast prevState back 
	// to unless I do something use typeid().name but then I'm still doing a 
	// check of all states.  So here's the enum list.  This comment can be 
	// deleted after reading, just thought I'd leave a note on what I've 
	// tried.
public:
	enum GState
	{
		MAINMENU, PAUSE, SCOREMENU, OPTIONS, LOADING, PLAY, SHOP, GAMEOVER, QUIT,
		STORY,NONE
	};

	StateCore(D3DGraphics &gfx, DSound& audio);
	~StateCore();

	D3DGraphics &gfx;
	DSound& audio;
	InputController ic;
	Player player;
	std::unique_ptr<Level> level;
	std::unique_ptr<Enemy> boss;
	std::vector<std::unique_ptr<Projectile>> ammo;
	std::vector<std::unique_ptr<Asteroid>> asteroid;
	std::vector<std::unique_ptr<Enemy>> enemy;
	ScoreController cScore;	
	GState curState, prevState;

	unsigned int currentLevel;
	unsigned int enemyCount[MAXENEMYTYPES];
	bool gameStarted;

};