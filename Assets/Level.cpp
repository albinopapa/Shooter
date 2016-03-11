#include "Level.h"
#include "StateCore.h"
#include "EntityController.h"

Level::Level(D3DGraphics &gfx, StateCore &core)
	:
	audFile(),
	currentTrigger( 1 ),
	aSpawn(),
	eBoss(),
	core(core),
	cam(0, 0, gfx.GetWidth(), gfx.GetHeight() )
{
}

LPCTSTR Level::GetBGAud()
{
	return audFile;
}

Camera *Level::GetCam() 
{
	return &cam;
}

void Level::CheckTriggers()
{
	for(int i = 0; i < numEnemyTriggers + 2; i++)
	{
		triggerList[ i ]->Do();
	}
}

unsigned int Level::GetLWidth() const
{
	return lWidth;
}

unsigned int Level::GetLHeight() const
{
	return lHeight;
}

void Level::InitCamera()
{
	D3DRECT rect = {0, lHeight - 1 - core.gfx.GetHeight(), lWidth, lHeight - 1};
	cam.SetRect( &rect);
}

void Level::InitTriggers()
{	
	unsigned int numTriggers = numEnemyTriggers + 2;
	triggerList.reset(new Trigger*[ numTriggers ]);
	
	eSpawn.reset(new std::unique_ptr<ETrigger>[numEnemyTriggers]);
	aSpawn = std::make_unique<ATrigger>(core);
	aSpawn->Set(rand() % 799, rand() % 2400 + 1200);
	aSpawn->Clear();

	eBoss = std::make_unique<BTrigger>(core);
	eBoss->Set(399, 300);
	eBoss->Clear();

	unsigned int i = 0;
	for(i = 0; i < numEnemyTriggers; i++)
	{
		eSpawn[i] = std::make_unique<ETrigger>(core);

		unsigned int j = (3000 / numTriggers) * (i+1);
		eSpawn[i]->SetEnemyType( (unsigned int)(rand() % numEnemySpawnTypes ) + 1);
		eSpawn[ i ]->Set( 0, lHeight - (core.gfx.GetHeight() * 2) - j);

		triggerList[i] = eSpawn[i].get();
		eSpawn[i]->Clear();
	}

	triggerList[i] = aSpawn.get();
	i++;
	triggerList[ i ] = eBoss.get();
	currentTrigger = 0;
	cur = triggerList[currentTrigger];
}

void Level::ClearTrigger(bool isCleared )
{
	if(isCleared)
	{
		currentTrigger++;
		if(currentTrigger < numEnemyTriggers + 2)
		{
			cur = triggerList[currentTrigger];
		}
	}
}

Level::~Level()
{
}

Level1::Level1(D3DGraphics &gfx, StateCore &core)
	:
	Level(gfx, core)
{	
	audFile = L"\\Audio\\Level01.wav";
	lHeight = core.gfx.GetHeight() * 7;
	lWidth = gfx.GetWidth();
	numEnemySpawnTypes = 4;
	numEnemyTriggers = 10;

	InitCamera();
	InitTriggers();
}

Level1::~Level1()
{
}

Level2::Level2(D3DGraphics &gfx, StateCore &core)
	:
	Level(gfx, core)
{
	audFile = L"\\Audio\\Level01.wav";
	lHeight = 4200;
	lWidth = gfx.GetWidth();
	numEnemySpawnTypes = 4;
	numEnemyTriggers = 10;

	InitCamera();
	InitTriggers();
}

Level2::~Level2()
{
}

Level3::Level3(D3DGraphics &gfx, StateCore &core)
	:
	Level(gfx, core)
{
	audFile = L"\\Audio\\Level01.wav";
	lHeight = 4200;
	lWidth = gfx.GetWidth();
	numEnemySpawnTypes = 4;
	numEnemyTriggers = 10;

	InitCamera();
	InitTriggers();
}

Level3::~Level3()
{
}

Level4::Level4(D3DGraphics &gfx, StateCore &core)
	:
	Level(gfx, core)
{
	audFile = L"\\Audio\\Level01.wav";
	lHeight = 4200;
	lWidth = gfx.GetWidth();
	numEnemySpawnTypes = 4;
	numEnemyTriggers = 10;

	InitCamera();
	InitTriggers();
}

Level4::~Level4()
{
}

Level5::Level5(D3DGraphics &gfx, StateCore &core)
	:
	Level(gfx, core)
{
	audFile = L"\\Audio\\Level01.wav";
	lHeight = 4200;
	lWidth = gfx.GetWidth();
	numEnemySpawnTypes = 4;
	numEnemyTriggers = 10;

	InitCamera();
	InitTriggers();
}

Level5::~Level5()
{
}

Level6::Level6(D3DGraphics &gfx, StateCore &core)
	:
	Level(gfx, core)
{
	audFile = L"\\Audio\\Level01.wav";
	lHeight = 4200;
	lWidth = gfx.GetWidth();
	numEnemySpawnTypes = 4;
	numEnemyTriggers = 10;

	InitCamera();
	InitTriggers();
}

Level6::~Level6()
{
}

Level7::Level7(D3DGraphics &gfx, StateCore &core)
	:
	Level(gfx, core)
{
	audFile = L"\\Audio\\Level01.wav";
	lHeight = 4200;
	lWidth = gfx.GetWidth();
	numEnemySpawnTypes = 4;
	numEnemyTriggers = 10;

	InitCamera();
	InitTriggers();
}

Level7::~Level7()
{
}

Level8::Level8(D3DGraphics &gfx, StateCore &core)
	:
	Level(gfx, core)
{
	audFile = L"\\Audio\\Level01.wav";
	lHeight = 4200;
	lWidth = gfx.GetWidth();
	numEnemySpawnTypes = 4;
	numEnemyTriggers = 10;

	InitCamera();
	InitTriggers();
}

Level8::~Level8()
{
}
