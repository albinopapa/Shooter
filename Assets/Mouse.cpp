#include "Mouse.h"

Mouse::Mouse(HWND hWnd)
	:
	Input(hWnd)	
{
	
	ZeroMemory( &dims, sizeof(dims) );
	hasDevice = Init( GUID_SysMouse, &c_dfDIMouse2, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

}

void Mouse::Update()
{
	Poll( sizeof(DIMOUSESTATE2), &dims);
}

short Mouse::GetX()
{
	return dims.lX;
}

short Mouse::GetY()
{
	return dims.lY;
}

short Mouse::GetWheel()
{
	return dims.lZ += dims.lZ;
}

bool Mouse::GetButtons( BYTE btn )
{
	return dims.rgbButtons[btn] & 0x80;
}

Mouse::~Mouse()
{
	if( pDevice )
	{
		pDevice->Unacquire();
		pDevice->Release();
	}
	pDevice = nullptr;
}