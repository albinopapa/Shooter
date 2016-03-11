#pragma once

#include "D3DGraphics.h"
#include "Vec2.h"

class Camera
{
public:
	Camera( int startX, int startY, unsigned int scrnWidth, unsigned int scrnHeight );
	D3DRECT GetViewport( );
	void Do();
	Vec2 GetPosition();
	float GetDistance();
	void SetRect( D3DRECT *rect);
private:
	void UpdateTimer();
	Vec2 pos;
	float speed, fTimer, dist;
	D3DRECT viewport;
	unsigned int width, height;
};