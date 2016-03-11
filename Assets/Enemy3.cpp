#include "Enemy3.h"
#include "StateCore.h"

Enemy3::Enemy3(StateCore &core)
	:
	Enemy(core)
{
	maxSpeed = 3;
	NPoints = 2;
	Points = std::make_unique<Vec2[]>(NPoints);
	pos = { 0.0f, 0.0f };
	Points[0] = { 0.0f, 0.0f };
	Points[1] = {
		core.gfx.GetWidth() + (radius * 2.0f),
		core.gfx.GetHeight() + (radius * 2.0f)
	};
	slope = { 0.0f, 0.0f };

	Nextp = &Points[1];

	numVerts = 8;
	verts = std::make_unique<Vec2[]>(numVerts);
	// Point 1
	verts[0] = { -7.0f, -15.0f };
	// Point 2
	verts[1] = { 7.0f, -15.0f };
	// Point 3
	verts[2] = { 16.0f, -7.0f };
	// Point 4
	verts[3] = { 16.0f, 7.0f };
	// Point 5
	verts[4] = { 7.0f, 16.0f };
	//Point 6
	verts[5] = { -7.0f, 16.0f };
	//Point 7
	verts[6] = { -15.0f, 7.0f };
	//Point 8
	verts[7] = { -15.0f, -7.0f };
}

Enemy3::~Enemy3()
{
}

