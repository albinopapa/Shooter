#include "Keyboard.h"

Keyboard::Keyboard(HWND hWnd)
	:
	Input(hWnd)
{
	ZeroMemory( keys, sizeof(BYTE)*256 );
	hasDevice = Init( GUID_SysKeyboard, &c_dfDIKeyboard, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
}

void Keyboard::Update()
{
	Poll( sizeof(BYTE)* 256, keys);
}

bool Keyboard::GetButtons( BYTE btn )
{
	return keys[ btn ] & 0x80;
}

Keyboard::~Keyboard()
{
	if( pDevice )
	{
		pDevice->Unacquire();
		pDevice->Release();
	}
	pDevice = nullptr;
}