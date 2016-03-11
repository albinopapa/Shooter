#include "Joystick.h"

Joystick::Joystick(HWND hWnd)
	:
	Input(hWnd)
{
	ZeroMemory( &diJState, sizeof(diJState) );
	hasDevice = Init( GUID_Joystick, &c_dfDIJoystick, DISCL_FOREGROUND|DISCL_EXCLUSIVE);
}

void Joystick::Update()
{
	Poll( sizeof(DIMOUSESTATE2), &diJState);
}

short Joystick::GetX()
{
	return diJState.lX;
}

short Joystick::GetY()
{
	return diJState.lY;
}

bool Joystick::GetButtons( BYTE btn )
{
	return diJState.rgbButtons[btn] & 0x80;
}

void Joystick::Normalize(float *x, float *y)
{
	*x = ( (float)diJState.lX - 32767) / 32767;
	*y = ( (float)diJState.lY - 32767) / 32767;
}

void Joystick::RemoveDeadZone(float *x, float *y)
{	
	float dz = 0.05f;
	if(*x <= dz && *x >= -dz)
	{
		*x = 0.0f;
	}
	if(*y <= dz && *y >= -dz)
	{
		*y = 0.0f;
	}
}

Joystick::~Joystick()
{
	if( pDevice )
	{
		pDevice->Unacquire();
		pDevice->Release();
	}
	pDevice = nullptr;
}