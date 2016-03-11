#pragma once

// #include <windows.h>
#include "Trigger.h"
#include "D3DGraphics.h"
#include "Camera.h"

class StateCore;

class Level
{
public:

	Level(D3DGraphics &gfx, StateCore &core);
	virtual ~Level();

protected:
	// Level init functions
	void InitCamera();
	void InitTriggers();
public:
	LPCTSTR GetBGAud();
	Camera *GetCam(); 
	void CheckTriggers();
	void ClearTrigger( bool passedTrigger );
	unsigned int GetLWidth() const;
	unsigned int GetLHeight() const;
	
protected:
	// File names of background image and background music
	LPCTSTR audFile;
	unsigned int numEnemySpawnTypes, numEnemyTriggers;
	ETrigger **eSpawn;
	Trigger *eBoss;
	Trigger *aSpawn;
	Trigger **triggerList;
	Trigger *cur;
	StateCore &core;
	unsigned int currentTrigger;
	unsigned int lWidth, lHeight;
	Camera cam;
};

class Level1 : public Level
{
public:
	Level1(D3DGraphics &gfx, StateCore &core);
	~Level1();
};

class Level2 : public Level
{
public:
	Level2(D3DGraphics &gfx, StateCore &core);
	~Level2();
};

class Level3 : public Level
{
public:
	Level3(D3DGraphics &gfx, StateCore &core);
	~Level3();
};

class Level4 : public Level
{
public:
	Level4(D3DGraphics &gfx, StateCore &core);
	~Level4();
};

class Level5 : public Level
{
public:
	Level5(D3DGraphics &gfx, StateCore &core);
	~Level5();
};

class Level6 : public Level
{
public:
	Level6(D3DGraphics &gfx, StateCore &core);
	~Level6();
};

class Level7 : public Level
{
public:
	Level7(D3DGraphics &gfx, StateCore &core);
	~Level7();
};

class Level8 : public Level
{
public:
	Level8(D3DGraphics &gfx, StateCore &core);
	~Level8();
};
