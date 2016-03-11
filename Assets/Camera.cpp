#include "Camera.h"
#include "TimerServer.h"

Camera::Camera( int startX, int startY, unsigned int scrnWidth, unsigned int scrnHeight)
{
	viewport.x1 = startX;
	viewport.y1 = startY;
	viewport.x2 = startX + scrnWidth;
	viewport.y2 = startY + scrnHeight;
	width = scrnWidth;
	height = scrnHeight;
	pos.x = startX;
	pos.y = startY;
	speed = 35;
	fTimer = 0;
	dist = 0.0f;
}

float Camera::GetDistance()
{
	return dist;
}

D3DRECT Camera::GetViewport()
{
	return viewport;	
}

void Camera::SetRect( D3DRECT *rect)
{
	viewport = *rect;
	pos.y = rect->y1;
}

void Camera::Do( )
{
	UpdateTimer( );
	dist = min( 1, CalcSpeed(  speed ));
	if(viewport.y1 - dist > 0)
	{
		pos.y -= dist;
	}
	else
	{
		dist = 0;
	}
	viewport.y1 = pos.y + 0.5f;
	viewport.y2 = viewport.y1 + height;
}

void Camera::UpdateTimer()
{
	TimerClient tc;
	fTimer = tc.GetFrameTime();
}

Vec2 Camera::GetPosition()
{
	return pos;
}

