#include "StoryLevel.h"

StoryLevel::StoryLevel(StateController &sc, StateCore& core)
	:
	GameState( sc, core ),
	story( core ),
	core( core )
{
	timer.StartWatch();
}

StoryLevel::~StoryLevel()
{}

void StoryLevel::RunEvents()
{
	ctime = timer.GetTimeMilli() / 1000;
	events.RunEvents();
}


void StoryLevel::BeginFrame()
{
	gfx.BeginFrame();
}

void StoryLevel::ComposeFrame()
{
	BeginFrame();
	RunEvents();
}

void StoryLevel::Do()
{
	ComposeFrame();
	if( events.NoEventsLeft() )
	{
		timer.StopWatch();
		core.prevState = core.curState;
		core.curState = core.PLAY;
	}
}

SLevel1::SLevel1(StateController &sc, StateCore& core)
	:
	StoryLevel( sc, core )
{
	//Events here
	/*events.NewEvent( &ctime, GreaterEqual, 5.0f, PlaySnd, false, 3, 1, "bobasarus", &story );*/
}

SLevel1::~SLevel1()
{}
