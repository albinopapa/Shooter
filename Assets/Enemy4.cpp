#include "Enemy4.h"
#include "StateCore.h"

Enemy4::Enemy4(StateCore &core)
	:
	Enemy(core)
{
	int center = core.gfx.GetWidth() / 2;
	int xGap = 50;
	int yGap = 100;
	maxSpeed = 3;
	NPoints = 7;
	Points = std::make_unique<Vec2[]>(NPoints);
	for (unsigned int i = 0; i < NPoints; i++)
	{
		xGap *= -1;
		Points[i] =
		{
			static_cast<float>(center + xGap),
			static_cast<float>(yGap * i)
		};
	}
	Nextp = &Points[1];
	slope = { 0.0f, 0.0f };

	numVerts = 7;
	verts = std::make_unique<Vec2[]>(numVerts);
	// Point 1
	verts[0] = { -5.0f, -15.0f };
	// Point 2
	verts[1] = { 0.0f, -5.0f };
	// Point 3
	verts[2] = { -11.0f, -5.0f };
	// Point 4
	verts[3] = { 0.0f, 16.0f };
	// Point 5
	verts[4] = { 11.0f, -5.0f };
	// Point 6
	verts[5] = verts[1];
	// Point 7
	verts[7] = { 5.0f, -15.0f };
}

Enemy4::~Enemy4()
{
}
