#pragma once
#include "Input.h"

class Mouse : public Input
{
public:
	Mouse(HWND hWnd);
	~Mouse();
	void Update();
	short GetX();
	short GetY();
	short GetWheel();
	bool GetButtons( BYTE btn);
private:
	DIMOUSESTATE2 dims;
};

