#include "StateController.h"
#include "Menu.h"
#include "Shop.h"
#include "WeaponController.h"
#include "Play.h"
#include "Loading.h"
#include "GameOver.h"
#include "StoryLevel.h"
#include <memory>

StateController::StateController(StateCore &core)
		:
	core(core),
	state(nullptr),
	isPaused(false),
	pauseState(nullptr),
	isShopping(false)
	{}

void StateController::Transition(std::unique_ptr<GameState> &pNewState)
{
	state = std::move(pNewState);
}

void StateController::IsInShop( bool Shopping )
{
	isShopping = Shopping;
}

void StateController::Do()
{
	if(core.curState == StateCore::GState::NONE)
	{
		core.curState = StateCore::GState::MAINMENU;
	}
	if(core.curState != core.prevState)
	{
		std::unique_ptr<GameState> temp_state;

		switch(core.curState)
		{
		case StateCore::GState::QUIT:
			PostQuitMessage(0);
			break;
		case StateCore::GState::MAINMENU:
			temp_state = std::make_unique<MainMenu>(*this, core, false);
			break;
		case StateCore::GState::PAUSE:
			temp_state = std::make_unique<MainMenu>(*this, core, true);
			break;
		case StateCore::GState::SCOREMENU:
			temp_state = std::make_unique<ScoreMenu>( *this, core );
			break;
		case StateCore::GState::LOADING:
			temp_state = std::make_unique<Loading>( *this, core);
			break;
		case StateCore::GState::PLAY:
			temp_state = std::make_unique<Play>( *this, core );
			core.prevState = core.PLAY;
			break;
		case StateCore::GState::SHOP:
			temp_state = std::make_unique<Shop>( *this, core );
			break;
		case StateCore::GState::GAMEOVER:
			temp_state = std::make_unique<GameOver>( *this, core);
			break;	
		case StateCore::GState::STORY:
			switch( GetLevelIndex() )
			{
			case 1:
				temp_state = std::make_unique<SLevel1>( *this, core);
				break;	
			}
		}
		Transition(temp_state);
	}
	
	state->Do();
}

void StateController::DoEscape()
{
	if(core.curState == StateCore::GState::PLAY)
	{
		core.prevState = core.curState;
		core.curState = StateCore::GState::PAUSE;
	}
	else if(core.curState == StateCore::GState::MAINMENU)
	{
		core.curState = StateCore::GState::QUIT;
		core.prevState = StateCore::GState::NONE;
	}
	else if(core.curState == StateCore::GState::PAUSE)
	{
		core.prevState = core.curState;
		core.curState = StateCore::GState::PLAY;
	}
	else if(core.curState == StateCore::GState::SCOREMENU || 
		core.curState == StateCore::GState::OPTIONS)
	{
		core.curState = core.prevState;
	}
	else if(core.curState == StateCore::GState::SHOP)
	{
		core.curState = StateCore::GState::PLAY;
	}
}

void StateController::DoF10()
{
	if(core.curState == StateCore::GState::PLAY)
	{
		core.prevState = core.curState;
		core.curState = StateCore::GState::SHOP;
	}
	else if(core.curState == StateCore::GState::SHOP)
	{
		core.curState = core.prevState;
	}
}

void StateController::SetLevelIndex( unsigned int i)
{
	core.currentLevel = i;
}

unsigned int StateController::GetLevelIndex()const
{
	return core.currentLevel;
}

Level *StateController::GetLevel() const
{
	return core.level.get();
}

void StateController::SetLevel(std::unique_ptr<Level> &NextLevel)
{	
	core.level = std::move(NextLevel);
}

StateController::~StateController()
{
	
}