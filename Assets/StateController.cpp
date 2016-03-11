#include "StateController.h"
#include "Menu.h"
#include "Shop.h"
#include "WeaponController.h"
#include "Play.h"
#include "Loading.h"
#include "GameOver.h"
#include "StoryLevel.h"

StateController::StateController(StateCore &core)
		:
	core(core),
	state(nullptr),
	isPaused(false),
	pauseState(nullptr),
	isShopping(false)
	{}

void StateController::Transition( GameState *pNew)
{
	if(state)
	{
		delete state;
	}

	state = pNew;
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
		switch(core.curState)
		{
		case StateCore::GState::QUIT:
			PostQuitMessage(0);
			break;
		case StateCore::GState::MAINMENU:
			Transition(new MainMenu(*this, core, false));
			break;
		case StateCore::GState::PAUSE:
			Transition(new MainMenu( *this, core, true));
			break;
		case StateCore::GState::SCOREMENU:
			Transition(new ScoreMenu( *this, core ) );
			break;
		case StateCore::GState::LOADING:
			Transition(new Loading( *this, core) );
			break;
		case StateCore::GState::PLAY:
			Transition(new Play( *this, core ) );
			core.prevState = core.PLAY;
			break;
		case StateCore::GState::SHOP:
			Transition(new Shop( *this, core ) );
			break;
		case StateCore::GState::GAMEOVER:
			Transition(new GameOver( *this, core) );
			break;	
		case StateCore::GState::STORY:
			switch( GetLevelIndex() )
			{
			case 1:
				Transition(new SLevel1( *this, core) );
				break;	
			}
		}
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
	return core.level;
}

void StateController::SetLevel( Level *l )
{
	if(core.level)
	{
		delete core.level;
		core.level = nullptr;
	}
	core.level = l;
}

StateController::~StateController()
{
	
}