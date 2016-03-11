#pragma once
#include "Input.h"

class Joystick : public Input
{
public:
	Joystick(HWND hWnd);
	~Joystick();
	void Update();
	short GetX();
	short GetY();
	bool GetButtons( BYTE btn);

	void Normalize(float *x, float *y);
	void RemoveDeadZone(float *x, float *y);
private:
	DIDEVCAPS diCaps;
	DIJOYSTATE diJState;
};