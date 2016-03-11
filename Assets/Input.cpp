#include "input.h"
#include <assert.h>


BOOL CALLBACK DIEnumDevices( LPCDIDEVICEINSTANCE lpddi, LPVOID lpvRef )
{
	// some complex pointer stuff huh xD
	// i pretty much convert the dwords to pointers
	LPDIRECTINPUTDEVICE8* lpPad = (LPDIRECTINPUTDEVICE8*)((DWORD*)lpvRef)[1];
	((LPDIRECTINPUT8)(*(DWORD*)lpvRef))->CreateDevice( lpddi->guidInstance, lpPad, NULL );

	DIDEVCAPS diCaps = *(LPDIDEVCAPS)(((DWORD*)lpvRef)[2]);
	ZeroMemory( &diCaps, sizeof(DIDEVCAPS) );
	(*lpPad)->GetCapabilities( &diCaps );
	if( diCaps.dwAxes < 4 )
		return DIENUM_CONTINUE;

	return DIENUM_STOP;
}

Input::Input(HWND hWnd)
	: hwnd( hWnd )
{
	HRESULT hr = 0;
	DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpdi, NULL );
}

bool Input::Init( const GUID &devGUID, const DIDATAFORMAT *pDevFormat, DWORD flags)
{
	pDevice = nullptr;
	lpdi->CreateDevice( devGUID, &pDevice, NULL );
	if(pDevice)
	{
		pDevice->SetDataFormat( pDevFormat );
		pDevice->SetCooperativeLevel( hwnd, flags );

		DIPROPDWORD dipd;
		dipd.diph.dwSize = sizeof(DIPROPDWORD);
		dipd.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipd.diph.dwObj = 0;
		dipd.diph.dwHow = DIPH_DEVICE;
		dipd.dwData = sizeof(DIDEVICEOBJECTDATA);
		pDevice->SetProperty( DIPROP_BUFFERSIZE, &dipd.diph );		
	}
	return pDevice != nullptr;
}

Input::~Input()
{
	if( pDevice )
	{
		pDevice->Unacquire();
		pDevice->Release();
	}
	pDevice = nullptr;

	if(lpdi)
	{
		lpdi->Release();
	}
	lpdi = nullptr;
}

void Input::Poll( DWORD bufSize, LPVOID stateBuffer )
{
	if(pDevice)
	{
		pDevice->Acquire();
		pDevice->Poll();
		pDevice->GetDeviceState( bufSize, stateBuffer );
	}
}

char Input::Read()
{
	DWORD dw = 1;
	DIDEVICEOBJECTDATA didod;
	pDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), &didod, &dw, 0 );
	return dw ? didod.dwOfs : 0;
}

bool Input::Peek()
{
	DWORD dw = 1;
	pDevice->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), NULL, &dw, DIGDD_PEEK );
	return dw;
}

bool Input::Check()
{
	return hasDevice;
}
