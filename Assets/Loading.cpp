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
	std::unique_ptr<Level> next_level;
	switch (sc.GetLevelIndex())
	{
	case 1:
		next_level = std::make_unique<Level1>(core.gfx, core);
		break;
	case 2:
		next_level = std::make_unique<Level2>(core.gfx, core);
		break;
	case 3:
		next_level = std::make_unique<Level3>(core.gfx, core);
		break;
	case 4:
		next_level = std::make_unique<Level4>(core.gfx, core);
		break;
	case 5:
		next_level = std::make_unique<Level5>(core.gfx, core);
		break;
	case 6:
		next_level = std::make_unique<Level6>(core.gfx, core);
		break;
	case 7:
		next_level = std::make_unique<Level7>(core.gfx, core);
		break;
	case 8:
		next_level = std::make_unique<Level8>(core.gfx, core);
		break;
	}
	sc.SetLevel(next_level);

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


