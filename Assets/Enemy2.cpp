#include "Enemy2.h"
#include "StateCore.h"

Enemy2::Enemy2(StateCore &core)
	:
	Enemy(core)
{
	maxSpeed = 3;
	NPoints = 3;
	
	Points = std::make_unique<Vec2[]>(NPoints);
	Points[0] = { 0.0f, 0.0f };
	Points[1] = { static_cast<float>(core.gfx.GetWidth() / 2), 
		static_cast<float>(core.gfx.GetHeight()) };
	Points[2] = { static_cast<float>(core.gfx.GetWidth()) + (radius * 2.0f), -(radius * 2.0f) };
	
	Nextp = &Points[1];

	numVerts = 4;
	verts = std::make_unique<Vec2[]>(numVerts);
	// Point 1
	verts[0] = Vec2(-15.0f, -15.f);
	// Point 2
	verts[1] = Vec2(15.0f, -15.0f);
	// Point 3
	verts[2] = Vec2(15.0f, 15.0f);
	// Point 4
	verts[3] = Vec2(-15.0f, 15.0f);
}

Enemy2::~Enemy2()
{
}

