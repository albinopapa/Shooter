#pragma once
#include "Timer.h"
#include "Event.h"
#include "Story.h"
#include "GameState.h"

class StoryLevel : public GameState
{
public:
	StoryLevel(StateController &sc, StateCore& core);
	~StoryLevel();
	void Do();
	void ComposeFrame();
	void BeginFrame();
private:
	Timer timer;
	StateCore& core;
protected:
	void RunEvents();
	Events events;
	Story story;
	float ctime;//current time
};

class SLevel1 : public StoryLevel
{
public:
	SLevel1(StateController &sc, StateCore& core);
	~SLevel1();
};