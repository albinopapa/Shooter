#pragma once

#include "GameState.h"
#include "Button.h"

class GameOver : public GameState
{
public:
	GameOver(StateController &sc, StateCore &core);
	void Do()override;
	void BeginFrame()override;
	void ComposeFrame()override;

private:
	std::string name;
	Button mQuit;
	Button mTryAgain;
};
