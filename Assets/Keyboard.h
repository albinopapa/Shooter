#pragma once
#include "Input.h"

class Keyboard : public Input
{
public:
	Keyboard(HWND hWnd);
	~Keyboard();
	void Update();
	bool GetButtons( BYTE btn );

private:
	BYTE keys[256];
};
