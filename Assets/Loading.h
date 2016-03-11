#pragma once

#include "GameState.h"

class Loading : public GameState
{
	// This class initializes Camera and loads a level
public:
	Loading(StateController &sc, StateCore &core);
	virtual void Do();
private:
	void LoadLevel();
	virtual void BeginFrame()
	{
		gfx.BeginFrame();
	}
	void ComposeFrame()
	{
		// Don't think the loading screen will be up long enough, but can draw anything to screen 
		// while level is loading.  Was thinking of randomly generating a multi layered starfield
		// for the background, using a surface.
	}
};
