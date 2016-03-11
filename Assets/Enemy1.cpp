#include "Enemy1.h"
#include "StateCore.h"

Enemy1::Enemy1(StateCore &core)
	:
	Enemy(core)
{
	maxSpeed = 3.0f;
	NPoints = 3;
	Points = std::make_unique<Vec2[]>(NPoints);
	Points[0] = { 0.0f, 0.0f };
	Points[1] = { static_cast<float>(core.gfx.GetWidth()), 0.0f };
	Points[2] = { static_cast<float>(core.gfx.GetWidth() / 2), static_cast<float>(core.gfx.GetHeight()) + radius };

	Nextp = &Points[1];
	slope.x = 0;
	slope.y = 0;
	numVerts = 4;
	verts = std::make_unique<Vec2[]>(numVerts);
	// Point 1
	verts[0] = Vec2(0.0f, -15.0f);
	// Point 2
	verts[1] = Vec2(-10.0f, 0.0f);
	// Point 3
	verts[2] = Vec2(0.0f, 15.0f);
	// Point 4
	verts[3] = Vec2(10.0f, 0.0f);
}

Enemy1::~Enemy1()
{
}
