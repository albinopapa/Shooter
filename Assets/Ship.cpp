#include "Ship.h"
#include "StateCore.h"

/*  The Ship class is the parent class and will store all functions that are not
	specific to any one ship and it will store virtual functions that will be
	defined in child classes.  Any data that is ship specific will be listed in
	the child class while common data such as speed will be stored in the parent
	class as protected with a Get function for retrieving the data.*/

/*  Ships can be created by listing the X and Y distance from the center of the ship
	in the constructor of the child class.
	If ships take up 32x32 then center would be x=15, y=15
	List the vertices either in a clockwise or anticlockwise manner, as long as each
	point connects, the DrawShip function in Game will connect the last point in the
	list to the first point. */

Ship::Ship(StateCore &core)
	:
	Entity(core)
{
}

void Ship::AdjustPosition( float camMoveDistance )
{
	pos.y -= camMoveDistance;
}

void Ship::Draw(const Vec2& camOffset)
{
	D3DGraphics::Color color = core.gfx.FILLCOLOR_XRGB(0, 255, 255);
	Vec2 ship_offset(WorldToScreen(pos, scrnSize, camOffset));

	for(unsigned int i = 0; i < numVerts; i++)
	{
		unsigned int j = (i+1) % numVerts;
		
		Vec2 vert_offset_i(ScreenRes(verts[i], scrnSize));
		Vec2 vert_offset_j(ScreenRes(verts[j], scrnSize));
		core.gfx.DrawShip(ship_offset, vert_offset_i, vert_offset_j, color);
	}
}

Ship::~Ship()
{
}

/*
TODO: Make more enemies types
*/
