#pragma once
#include <dinput.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input 
{
public:
	Input(HWND hWnd);
	virtual ~Input();
	// Derived class shared functions
	char Read();
	bool Peek();
	virtual bool GetButtons( BYTE ) = 0;
	virtual void Update() = 0;
	bool Check();
	
protected:
	void Poll( DWORD bufSize, LPVOID stateBuffer);
	bool Init( const GUID &devGuid, const DIDATAFORMAT *pDevFormat, DWORD flags);

protected:
	IDirectInput8 *lpdi;
	LPDIRECTINPUTDEVICE8 pDevice;
	HWND hwnd;
	bool hasDevice;
};


