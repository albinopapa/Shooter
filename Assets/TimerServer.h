#pragma once
#include "Timer.h"

class TimerClient;

class TimerServer
{
	friend TimerClient;
public:
	TimerServer( );
	static void Update();

protected:
	static Timer t;
	static float frame;

};


class TimerClient
{	
public:
	TimerClient(  );
	float GetFrameTime() const;
private:
	TimerServer tServ;
};

