#include "Vec2.h"

#include <math.h>
#include "TimerServer.h"
#include "D3DGraphics.h"

float CalcSpeed(  float speed )
{
	TimerClient tc;
	return speed * tc.GetFrameTime();
}

float CalcSlopeToAngle( float slope )
{
	return atan(slope);
}

Vec2 CalcAngleToSlope( int angle )
{	
	Vec2 c;
	angle %= 360;
		
	if(angle == 0 || angle == 360)
	{
		c.x = 0;
		c.y = -1;
	}
	else if( 0 < angle && angle < 45 )
	{
		c.x = tan(CalcDegToRad(angle));
		c.y = -1;
	}
	else if( angle == 45 )
	{
		c.x = 1;
		c.y = -1;
	}
	else if(  45 < angle && angle < 90 )
	{
		c.x = 1;
		c.y = tan(CalcDegToRad( angle - 90) );
	}	
	else if(angle == 90)
	{
		c.x = 1;
		c.y = 0;
	}
	else if( angle > 90 &&  angle < 135 )
	{
		c.x = 1;
		c.y = tan(CalcDegToRad( angle - 90) );
	}
	else if(angle == 135 )
	{
		c.x = 1;
		c.y = 1;
	}
	else if( angle > 135 &&  angle < 180 )
	{
		c.x = tan( CalcDegToRad( 180 - angle ) );
		c.y = 1;
	}
	else if(angle == 180)
	{
		c.x = 0;
		c.y = 1;
	}
	else if(angle > 180 &&  angle < 225 )
	{
		c.x = -tan(CalcDegToRad( angle ));
		c.y = 1;
	}
	else if( angle == 225 )
	{
		c.x = -1;
		c.y = 1;
	}
	else if(225 < angle && angle < 270)
	{
		c.x = -1;
		c.y = tan(CalcDegToRad( 270 - angle ) );
	}
	else if(angle == 270)
	{
		c.x = -1;
		c.y = 0;
	}
	else if(270 < angle && angle < 315 )
	{
		c.x = -1;
		c.y = -tan( CalcDegToRad( angle - 270 ) );
	}
	else if(angle == 315 )
	{
		c.x = -1;
		c.y = -1;
	}
	else if( 315 < angle && angle < 360 )
	{
		c.x = tan( CalcDegToRad( angle - 180) );
		c.y = -1;
	}
	return c;
}

float CalcDegToRad( int angleDegree )
{
	return static_cast<float>(angleDegree) * (3.14159f / 180.0f);
}

Vec2 CalcScreenScale(const Vec2& scrn)
{	
	return Vec2(scrn.x / (float)SCREENWIDTH, scrn.y / (float)SCREENHEIGHT );
}

Vec2 Vec2ScreenRes(const Vec2& Coord, const Vec2& scrn )
{
	Vec2 scale = CalcScreenScale( scrn );
	scale.x *= Coord.x;
	scale.y *= Coord.y;
	return scale;
}

Vec2 GameToScreenVec2(const Vec2& Coord, const Vec2& scrn, const Vec2& cam )
{
	Vec2 c(Coord.x - cam.x, Coord.y - cam.y);
	c = Vec2ScreenRes( c, scrn );
	return c;
}

Vec2 ScreenToGameVec2(const Vec2& Coord, const Vec2& scrn, const Vec2& cam )
{
	Vec2 c = Vec2ScreenRes(Coord, scrn );
	c.x = cam.x + Coord.x;
	c.y = cam.y + Coord.y;
	return c;
}
