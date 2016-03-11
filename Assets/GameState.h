#pragma once

#include <vector>
#include "StateController.h"

class GameState
{
public:
	GameState(StateController &sc, StateCore &core);
	virtual ~GameState();
	virtual void Do() = 0;
	virtual void BeginFrame() = 0;
	virtual void ComposeFrame() = 0;
protected:
	D3DGraphics::Font f;
	StateController &sc;
	StateCore &core;
	InputController &ic;
	D3DGraphics &gfx;
};

