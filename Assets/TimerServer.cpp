#include "TimerServer.h"

Timer TimerServer::t;
float TimerServer::frame = 0;

TimerServer::TimerServer()
{
	t.StartWatch(); 
}

void TimerServer:: Update() 
{
	t.StopWatch();
	frame = t.GetTimeMilli() * 0.001;
	t.StartWatch(); 
}

TimerClient::TimerClient( )
{}

float TimerClient::GetFrameTime() const 
{
	return tServ.frame;
}



