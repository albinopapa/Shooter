#pragma once

#include "Mouse.h"
#include "Keyboard.h"
#include "Joystick.h"
#include "Vec2.h"
#include "D3DGraphics.h"

class InputController
{
public:
	InputController( D3DGraphics &gfx );
	~InputController();
	void Update();
	bool Firing();

	// Mouse functions
	Vec2 GetMousePos();
	void SetClip( const Vec2 &mClip );
	bool LeftButtonClicked();
	void ClipMouse();

	bool IsPaused();
	bool KeyTyped( unsigned char key);
	void GetAxes( float* x, float* y );
private:
	void ClipAxis( unsigned int maxClipSize, float* axis );

	Mouse mouse;
	Keyboard kbd;
	Joystick jStick;
	D3DGraphics &gfx;
	
	Vec2 mPos;
	Vec2 jPos;
	Vec2 mBounds;
	
	bool mCLF, kbCLF, paused;
};