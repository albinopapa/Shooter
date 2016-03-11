#include "Loading.h"
#include "Play.h"

Loading::Loading(StateController &sc, StateCore &core)
:
GameState(sc, core)
{
}

void Loading::Do()
{
	BeginFrame();
	switch (sc.GetLevelIndex())
	{
	case 1:		
		sc.SetLevel(new Level1(core.gfx, core));
		break;
	case 2:
		sc.SetLevel(new Level2(core.gfx, core));
		break;
	case 3:
		sc.SetLevel(new Level3(core.gfx, core));
		break;
	case 4:
		sc.SetLevel(new Level4(core.gfx, core));
		break;
	case 5:
		sc.SetLevel(new Level5(core.gfx, core));
		break;
	case 6:
		sc.SetLevel(new Level6(core.gfx, core));
		break;
	case 7:
		sc.SetLevel(new Level7(core.gfx, core));
		break;
	case 8:
		sc.SetLevel(new Level8(core.gfx, core));
		break;
	}
	LoadLevel();
	ComposeFrame();

	core.prevState = core.curState;
	core.curState = StateCore::GState::STORY;
}


void Loading::LoadLevel()
{
	// Set player stats
	Vec2 camPos = core.level->GetCam()->GetPosition();
	core.player.SetPosition({ camPos.x + (core.gfx.GetWidth() / 2), camPos.y + (core.gfx.GetHeight() / 2) });
}


