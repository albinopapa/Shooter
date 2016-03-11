#pragma once

#include "StandardLibraryIncludes.h"

#include "Shield.h"
#include "InputController.h"
#include "Vec2.h"

class Ship : public Entity
{
public:
	Ship();
	Ship(StateCore &core);
	virtual ~Ship();

	enum ShipState
	{
		Moving,
		Firing,
		Dead
	};

	void Do() = 0;
	void AdjustPosition( float camMoveDistance );
	void Draw(const Vec2& camOffset);
protected:
	unsigned int numVerts;
	std::unique_ptr<Vec2[]> verts;

};

