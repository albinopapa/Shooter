#include "Enemy5.h"
#include "StateCore.h"

Enemy5::Enemy5(StateCore &core)
	:
	Enemy(core)
{
	maxSpeed = 3;
	NPoints = 5;
	Points = std::make_unique<Vec2[]>(NPoints);

	float scrWidth = static_cast<float>(core.gfx.GetWidth());
	float scrHeight = static_cast<float>(core.gfx.GetHeight());

	Points[0] = { scrWidth * 0.5f, 0.0f };
	Points[1] = { 0.0f, scrHeight * 0.5f };
	Points[2] = { scrWidth * 0.5f, scrHeight };
	Points[3] = { scrWidth, scrHeight * 0.5f };
	Points[4] = { (scrWidth * 0.5f) + (radius * 2.0f), -(radius * 2.0f) };

	Nextp = &Points[1];
	slope = { 0.0f, 0.0f };

	numVerts = 10;
	verts = std::make_unique<Vec2[]>(numVerts);
	// Point 1
	verts[0] = { -9.0f, -15.0f };
	// Point 2
	verts[1] = { -6.0f, -3.0f };
	// Point 3
	verts[2] = { -15.0f, 4.0f };
	// Point 4
	verts[3] = { -3.0f, 4.0f };
	// Point 5
	verts[4] = { 0.0f, 16.0f };
	// Point 6
	verts[5] = { 4.0f, 4.0f };
	// Point 7
	verts[6] = { 16.0f, 4.0f };
	// Point 8
	verts[7] = { 7.0f, -3.0f };
	// Point 9
	verts[8] = { 10.0f, -15.0f };
	// Point 10
	verts[9] = { 0.0f, -8.0f };
}

Enemy5::~Enemy5()
{
}
