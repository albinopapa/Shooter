#include "GameOver.h"
#include "Button.h"
#include "Loading.h"

GameOver::GameOver(StateController &sc, StateCore &core)
:
GameState(sc, core),
mQuit({ 255, 255, 255, 255 }, SCREENWIDTH / 2 - 50, SCREENHEIGHT / 2 - 25,
	SCREENWIDTH / 2 + 50, SCREENHEIGHT / 2 + 25, std::string("Quit?")),
	mTryAgain({ 255, 255, 255, 255 }, SCREENWIDTH / 2 - 50, SCREENHEIGHT / 2 - 25,
		SCREENWIDTH / 2 + 50, SCREENHEIGHT / 2 + 25, std::string("Try Again?"))
{
	
	/*scoreboard.AddScore( "Fred", scoreboard.GetTime(), 15 );
	scoreboard.AddScore( "George", scoreboard.GetTime(), 50 );
	scoreboard.AddScore( "Alex", scoreboard.GetTime(), 3 );
	scoreboard.AddScore( "Luis", scoreboard.GetTime(), 42 );
	scoreboard.AddScore( "Josh", scoreboard.GetTime(), 43 );
	scoreboard.SaveScoreList();*/
}

void GameOver::Do()
{	
	BeginFrame();
	if (mQuit.ButtonClicked(ic, gfx))
	{

		core.cScore.SaveList();
		PostQuitMessage(0);
	}
	else if (mTryAgain.ButtonClicked(ic, gfx))
	{
		sc.Transition(new Loading(sc, core));
	}
	ComposeFrame();

}

void GameOver::BeginFrame()
{
	gfx.BeginFrame();
}

void GameOver::ComposeFrame()
{
	gfx.DrawCircle(ic.GetMousePos().x, ic.GetMousePos().y, 32, gfx.FILLCOLOR_XRGB(255, 255, 255));
	mQuit.Draw(f, gfx);
	mTryAgain.Draw(f, gfx);
}