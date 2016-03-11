#include <string>
#include "D3DGraphics.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "FrameTimer.h"
#include <gdiplus.h>
#include <typeinfo>

D3DGraphics::D3DGraphics( HWND hWnd )
	:
pDirect3D( NULL ),
pDevice( NULL ),
pBackBuffer( NULL ),
pTestSurface( NULL )
{
	HRESULT result;

	backRect.pBits = NULL;

	pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
	assert( pDirect3D != NULL );

    ZeroMemory( &d3dpp,sizeof( d3dpp ) );
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    if( FAILED(result = pDirect3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,&d3dpp,&pDevice )) )
	{
		OutputDebugString( L"Failed to load HW only device, will try Mixed mode" );
		if( FAILED(result = pDirect3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,&d3dpp,&pDevice )) )
		{
			OutputDebugString( L"Failed to load Mixed mode device, will try Software mode" );
			if( FAILED(result = pDirect3D->CreateDevice( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,&d3dpp,&pDevice )) )
				assert( !FAILED( result ) );
		}
	}

	D3DVIEWPORT9 vp = { 0, 0, d3dpp.BackBufferWidth, d3dpp.BackBufferHeight, 0.f, 1.f };
	pDevice->SetViewport( &vp );

	result = pDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer );
	assert( !FAILED( result ) );

	pTestSurface = new Color[d3dpp.BackBufferWidth * d3dpp.BackBufferHeight];	
}

bool D3DGraphics::IsWindowed() const
{
	return d3dpp.Windowed;
}

HWND D3DGraphics::GetWindow() const
{
	return d3dpp.hDeviceWindow;
}

unsigned D3DGraphics::GetWidth() const
{
	return d3dpp.BackBufferWidth;
}

unsigned D3DGraphics::GetHeight() const
{
	return d3dpp.BackBufferHeight;
}

bool D3DGraphics::Reset()
{
	HRESULT hr(0);

	if( pBackBuffer )
		pBackBuffer->Release();
	if( pTestSurface )
		delete [] pTestSurface;

	if( d3dpp.Windowed )
		d3dpp.BackBufferWidth = d3dpp.BackBufferHeight = 0;

	if( pDevice )
		hr = pDevice->Reset( &d3dpp );

	if( FAILED(hr) )
		return false;

	pDevice->GetBackBuffer( 0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer );
	pTestSurface = new Color[d3dpp.BackBufferWidth * d3dpp.BackBufferHeight];
	return true;
}

void D3DGraphics::ToggleMode()
{
	d3dpp.Windowed = !d3dpp.Windowed;
	if( !d3dpp.Windowed ) {
		d3dpp.BackBufferWidth = GetSystemMetrics( SM_CXSCREEN );
		d3dpp.BackBufferHeight = GetSystemMetrics( SM_CYSCREEN );
		SetWindowLongPtr( d3dpp.hDeviceWindow, GWL_STYLE, WS_POPUPWINDOW );
		SetWindowPos( d3dpp.hDeviceWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED );
	} else {
		SetWindowLongPtr( d3dpp.hDeviceWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW );
		SetWindowPos( d3dpp.hDeviceWindow, HWND_NOTOPMOST, 300, 150, SCREENWIDTH, SCREENHEIGHT, SWP_SHOWWINDOW | SWP_FRAMECHANGED );
	}
}

D3DGraphics::~D3DGraphics()
{
	if( pDevice )
	{
		pDevice->Release();
		pDevice = NULL;
	}
	if( pDirect3D )
	{
		pDirect3D->Release();
		pDirect3D = NULL;
	}
	if( pBackBuffer )
	{
		pBackBuffer->Release();
		pBackBuffer = NULL;
	}
	if( pTestSurface )
	{
		delete pTestSurface;
		pTestSurface = NULL;
	}
}

void D3DGraphics::LoadVFont()
{
//	Color c = FILLCOLOR_XRGB(255, 255, 255);
//	// !
//	// "
//	// #
//	// $
//	// %
//	// &
//	// '
//	// (
//	// )
//	// *
//	// +
//	// ,
//	// -
//	// .
//	// /
//	// 0
//	char i = 0;
//
//	// pPoint 0
//	vFont[16].pPoint[i].x = 2;
//	vFont[16].pPoint[i].y = 0;
//	i++;
//	// pPoint 1
//	vFont[16].pPoint[i].x = 2;
//	vFont[16].pPoint[i].y = 15;
//	i++;
//	// pPoint 2
//	vFont[16].pPoint[i].x = 12;
//	vFont[16].pPoint[i].y = 15;
//	i++;
//	// pPoint 3
//	vFont[16].pPoint[i].x = 12;
//	vFont[16].pPoint[i].y = 31;
//	i++;
//	// pPoint 4
//	vFont[16].pPoint[i].x = 2;
//	vFont[16].pPoint[i].y = 31;
//
//	DrawLine(vFont->pPoint[0].x, vFont->pPoint[0].y, vFont->pPoint[1].x, vFont->pPoint[1].y, c);
//	DrawLine(vFont->pPoint[1].x, vFont->pPoint[1].y, vFont->pPoint[2].x, vFont->pPoint[2].y, c);
//	DrawLine(vFont->pPoint[2].x, vFont->pPoint[2].y, vFont->pPoint[3].x, vFont->pPoint[3].y, c);
//	DrawLine(vFont->pPoint[3].x, vFont->pPoint[3].y, vFont->pPoint[4].x, vFont->pPoint[4].y, c);
//	DrawLine(vFont->pPoint[4].x, vFont->pPoint[4].y, vFont->pPoint[1].x, vFont->pPoint[1].y, c);
//}
//case '1':
//{
//	char pPoint[2][5];
//	char x = 0;
//	char y = 1;
//	char i = 0;
//	// pPoint 0
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 0;
//	i++;
//	// pPoint 1
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 2
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 3
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 31;
//	i++;
//	// pPoint 4
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 31;
//	sizeof...;
//	DrawLine(pPoint[x][0], pPoint[y][0], pPoint[x][1], pPoint[y][1], 128, 255, 128 ); //
//	DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], 128, 255, 128 ); // |_
//	DrawLine(pPoint[x][2], pPoint[y][2], pPoint[x][3], pPoint[y][3], 128, 255, 128 ); // |_|
//	DrawLine(pPoint[x][3], pPoint[y][3], pPoint[x][4], pPoint[y][4], 128, 255, 128 );
//	DrawLine(pPoint[x][4], pPoint[y][4], pPoint[x][1], pPoint[y][1], 128, 255, 128 );
//	break;
//}
//case '2':
//{
//	char pPoint[2][5];
//	char x = 0;
//	char y = 1;
//	char i = 0;
//	// pPoint 0
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 0;
//	i++;
//	// pPoint 1
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 2
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 3
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 31;
//	i++;
//	// pPoint 4
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 31;
//
//	DrawLine(pPoint[x][0], pPoint[y][0], pPoint[x][1], pPoint[y][1], 128, 255, 128 ); //
//	DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], 128, 255, 128 ); // |_
//	DrawLine(pPoint[x][2], pPoint[y][2], pPoint[x][3], pPoint[y][3], 128, 255, 128 ); // |_|
//	DrawLine(pPoint[x][3], pPoint[y][3], pPoint[x][4], pPoint[y][4], 128, 255, 128 );
//	DrawLine(pPoint[x][4], pPoint[y][4], pPoint[x][1], pPoint[y][1], 128, 255, 128 );
//	break;
//}
//case '3':
//{
//	char pPoint[2][5];
//	char x = 0;
//	char y = 1;
//	char i = 0;
//	// pPoint 0
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 0;
//	i++;
//	// pPoint 1
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 2
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 3
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 31;
//	i++;
//	// pPoint 4
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 31;
//
//	DrawLine(pPoint[x][0], pPoint[y][0], pPoint[x][1], pPoint[y][1], 128, 255, 128 ); //
//	DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], 128, 255, 128 ); // |_
//	DrawLine(pPoint[x][2], pPoint[y][2], pPoint[x][3], pPoint[y][3], 128, 255, 128 ); // |_|
//	DrawLine(pPoint[x][3], pPoint[y][3], pPoint[x][4], pPoint[y][4], 128, 255, 128 );
//	DrawLine(pPoint[x][4], pPoint[y][4], pPoint[x][1], pPoint[y][1], 128, 255, 128 );
//	break;
//}
//case '4':
//{
//	char pPoint[2][5];
//	char x = 0;
//	char y = 1;
//	char i = 0;
//	// pPoint 0
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 0;
//	i++;
//	// pPoint 1
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 2
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 3
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 31;
//	i++;
//	// pPoint 4
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 31;
//
//	DrawLine(pPoint[x][0], pPoint[y][0], pPoint[x][1], pPoint[y][1], 128, 255, 128 ); //
//	DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], 128, 255, 128 ); // |_
//	DrawLine(pPoint[x][2], pPoint[y][2], pPoint[x][3], pPoint[y][3], 128, 255, 128 ); // |_|
//	DrawLine(pPoint[x][3], pPoint[y][3], pPoint[x][4], pPoint[y][4], 128, 255, 128 );
//	DrawLine(pPoint[x][4], pPoint[y][4], pPoint[x][1], pPoint[y][1], 128, 255, 128 );
//	break;
//}
//case '5':
//{
//	char pPoint[2][5];
//	char x = 0;
//	char y = 1;
//	char i = 0;
//	// pPoint 0
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 0;
//	i++;
//	// pPoint 1
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 2
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 3
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 31;
//	i++;
//	// pPoint 4
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 31;
//
//	DrawLine(pPoint[x][0], pPoint[y][0], pPoint[x][1], pPoint[y][1], 128, 255, 128 ); //
//	DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], 128, 255, 128 ); // |_
//	DrawLine(pPoint[x][2], pPoint[y][2], pPoint[x][3], pPoint[y][3], 128, 255, 128 ); // |_|
//	DrawLine(pPoint[x][3], pPoint[y][3], pPoint[x][4], pPoint[y][4], 128, 255, 128 );
//	DrawLine(pPoint[x][4], pPoint[y][4], pPoint[x][1], pPoint[y][1], 128, 255, 128 );
//	break;
//}
//case '6':
//{
//	char pPoint[2][5];
//	char x = 0;
//	char y = 1;
//	char i = 0;
//	// pPoint 0
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 0;
//	i++;
//	// pPoint 1
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 2
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 3
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 31;
//	i++;
//	// pPoint 4
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 31;
//
//	DrawLine(pPoint[x][0], pPoint[y][0], pPoint[x][1], pPoint[y][1], 128, 255, 128 ); //
//	DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], 128, 255, 128 ); // |_
//	DrawLine(pPoint[x][2], pPoint[y][2], pPoint[x][3], pPoint[y][3], 128, 255, 128 ); // |_|
//	DrawLine(pPoint[x][3], pPoint[y][3], pPoint[x][4], pPoint[y][4], 128, 255, 128 );
//	DrawLine(pPoint[x][4], pPoint[y][4], pPoint[x][1], pPoint[y][1], 128, 255, 128 );
//	break;
//case '7':
//	char pPoint[2][5];
//	char x = 0;
//	char y = 1;
//	char i = 0;
//	// pPoint 0
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 0;
//	i++;
//	// pPoint 1
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 2
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 3
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 31;
//	i++;
//	// pPoint 4
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 31;
//
//	DrawLine(pPoint[x][0], pPoint[y][0], pPoint[x][1], pPoint[y][1], 128, 255, 128 ); //
//	DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], 128, 255, 128 ); // |_
//	DrawLine(pPoint[x][2], pPoint[y][2], pPoint[x][3], pPoint[y][3], 128, 255, 128 ); // |_|
//	DrawLine(pPoint[x][3], pPoint[y][3], pPoint[x][4], pPoint[y][4], 128, 255, 128 );
//	DrawLine(pPoint[x][4], pPoint[y][4], pPoint[x][1], pPoint[y][1], 128, 255, 128 );
//	break;
//case '8':
//	char pPoint[2][5];
//	char x = 0;
//	char y = 1;
//	char i = 0;
//	// pPoint 0
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 0;
//	i++;
//	// pPoint 1
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 2
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 3
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 31;
//	i++;
//	// pPoint 4
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 31;
//
//	DrawLine(pPoint[x][0], pPoint[y][0], pPoint[x][1], pPoint[y][1], 128, 255, 128 ); //
//	DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], 128, 255, 128 ); // |_
//	DrawLine(pPoint[x][2], pPoint[y][2], pPoint[x][3], pPoint[y][3], 128, 255, 128 ); // |_|
//	DrawLine(pPoint[x][3], pPoint[y][3], pPoint[x][4], pPoint[y][4], 128, 255, 128 );
//	DrawLine(pPoint[x][4], pPoint[y][4], pPoint[x][1], pPoint[y][1], 128, 255, 128 );
//	break;
//case '9':
//	char pPoint[2][5];
//	char x = 0;
//	char y = 1;
//	char i = 0;
//	// pPoint 0
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 0;
//	i++;
//	// pPoint 1
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 2
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 15;
//	i++;
//	// pPoint 3
//	pPoint[x][i] = 12;
//	pPoint[y][i] = 31;
//	i++;
//	// pPoint 4
//	pPoint[x][i] = 2;
//	pPoint[y][i] = 31;
//
//	DrawLine(pPoint[x][0], pPoint[y][0], pPoint[x][1], pPoint[y][1], 128, 255, 128 ); //
//	DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], 128, 255, 128 ); // |_
//	DrawLine(pPoint[x][2], pPoint[y][2], pPoint[x][3], pPoint[y][3], 128, 255, 128 ); // |_|
//	DrawLine(pPoint[x][3], pPoint[y][3], pPoint[x][4], pPoint[y][4], 128, 255, 128 );
//	DrawLine(pPoint[x][4], pPoint[y][4], pPoint[x][1], pPoint[y][1], 128, 255, 128 );
//	break;
//
//
}

void D3DGraphics::DrawLetter(char letter)
{
	//Color c = FILLCOLOR_XRGB(128, 255, 128);
	//switch (letter)
	//{
	//case 'A':
	//	{
	//		char i = 0;
	//		// Point 0
	//		pPoint[i].x = 0;
	//		pPoint[i].y = 31;
	//		i++;
	//		// Point 1
	//		pPoint[i].x = 13;
	//		pPoint[i].y = 0;
	//		i++;
	//		// Point 2
	//		pPoint[i].x = 24;
	//		pPoint[i].y = 31;
	//		i++;
	//		// Point 3
	//		pPoint[i].x = 5;
	//		pPoint[i].y = 22;
	//		i++;
	//		// Point 4
	//		pPoint[i].x = 21;
	//		pPoint[i].y = 22;

	//		DrawLine(pPoint[0].x, pPoint[0].y, pPoint[1].x, pPoint[y][1], c);
	//		DrawLine(pPoint[x][1], pPoint[y][1], pPoint[x][2], pPoint[y][2], c);
	//		DrawLine(pPoint[x][3], pPoint[y][3], Point[x][4], pPoint[y][4], c);
	//		break;
	//	}
	//case 'a':
	//	{
	//		char Point[2][6];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 16;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 16;
	//		Point[y][i] = 22;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 0;
	//		Point[y][i] = 22;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 0;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 5
	//		Point[x][i] = 16;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 );
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 );
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 );
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
	//		DrawLine(Point[x][5], Point[y][5], Point[x][2], Point[y][2], 128, 255, 128 );
	//		break;
	//	}
	//case 'B':
	//	{
	//		char Point[2][6];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 16;
	//		Point[y][i] = 7;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 16;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 15;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 5
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  _
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][0], Point[y][0], 128, 255, 128 );
	//		DrawLine(Point[x][2], Point[y][2], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
	//		DrawLine(Point[x][5], Point[y][5], Point[x][3], Point[y][3], 128, 255, 128 );

	//		break;
	//	}
	//case 'b':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );

	//		break;
	//	}
	//case 'C':
	//	{
	//		char Point[2][6];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 31;
	//		Point[y][i] = 10;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 15;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 2;
	//		Point[y][i] = 10;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 2;
	//		Point[y][i] = 21;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 15;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 5
	//		Point[x][i] = 31;
	//		Point[y][i] = 21;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
	//		break;
	//	}
	//case 'c':
	//	{
	//		char Point[2][6];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 15;
	//		Point[y][i] = 21;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 7;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 2;
	//		Point[y][i] = 21;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 2;
	//		Point[y][i] = 26;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 7;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 5
	//		Point[x][i] = 15;
	//		Point[y][i] = 26;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
	//		break;
	//	}
	//case 'D':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 15;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 31;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 15;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'd':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 15;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 15;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 2;
	//		Point[y][i] = 16;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 15;
	//		Point[y][i] = 16;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		break;
	//	}
	//case 'E':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 31;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 23;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 5
	//		Point[x][i] = 31;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][2], Point[y][2], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][5], Point[y][5], 128, 255, 128 );
	//		break;
	//	}
	//case 'e':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'F':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'f':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'G':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'g':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'H':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'h':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'I':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'i':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'J':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'j':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'K':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'k':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'L':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'l':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'M':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'm':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'N':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'n':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'O':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'o':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'P':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'p':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'Q':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'q':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'R':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'r':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //  
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'S':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 's':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'T':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 't':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'U':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'u':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'V':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'v':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'W':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'w':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'X':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'x':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'Y':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'y':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'Z':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//case 'z':
	//	{
	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
	//		break;
	//	}
	//}

	//		char Point[2][5];
	//		char x = 0;
	//		char y = 1;
	//		char i = 0;
	//		// Point 0
	//		Point[x][i] = 2;
	//		Point[y][i] = 0;
	//		i++;
	//		// Point 1
	//		Point[x][i] = 2;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 2
	//		Point[x][i] = 12;
	//		Point[y][i] = 15;
	//		i++;
	//		// Point 3
	//		Point[x][i] = 12;
	//		Point[y][i] = 31;
	//		i++;
	//		// Point 4
	//		Point[x][i] = 2;
	//		Point[y][i] = 31;

	//		DrawLine(Point[x][0], Point[y][0], Point[x][1], Point[y][1], 128, 255, 128 ); //
	//		DrawLine(Point[x][1], Point[y][1], Point[x][2], Point[y][2], 128, 255, 128 ); // |_
	//		DrawLine(Point[x][2], Point[y][2], Point[x][3], Point[y][3], 128, 255, 128 ); // |_|
	//		DrawLine(Point[x][3], Point[y][3], Point[x][4], Point[y][4], 128, 255, 128 );
	//		DrawLine(Point[x][4], Point[y][4], Point[x][1], Point[y][1], 128, 255, 128 );
}

void D3DGraphics::PutPixel( unsigned int x, unsigned int y, Color c )
{
	if( x < 0 || y < 0 || x >= d3dpp.BackBufferWidth || y >= d3dpp.BackBufferHeight )
	{
		return;
	}
	else
	{
		if(c.A > 32 && c.A < 255)
		{
			pTestSurface[ x + y * d3dpp.BackBufferWidth ] = Blend(GetPixel(x, y), c);
		}
		else if(c.A == 255)
		{
			pTestSurface[ x + y * d3dpp.BackBufferWidth ] = c;
		}
	}
}

void D3DGraphics::PutPixel( Vec2& c, Color color )
{
	if( c.x < 0 || c.y < 0 || c.x >= d3dpp.BackBufferWidth || c.y >= d3dpp.BackBufferHeight )
	{
		return;
	}
	else
	{
		int i = (int)c.x + (int)c.y * d3dpp.BackBufferWidth;
		if(color.A > 32 && color.A < 255)
		{
			pTestSurface[ i ] = Blend(GetPixel(c.x, c.y), color);
		}
		else if(color.A == 255)
		{
			pTestSurface[ i ] = color;
		}
	}
}

D3DGraphics::Color D3DGraphics::GetPixel( int x,int y ) const
{
	return pTestSurface[ x + y * d3dpp.BackBufferWidth ];
}

void D3DGraphics::BeginFrame()
{
	memset( pTestSurface, FILLVALUE, sizeof( Color ) * d3dpp.BackBufferWidth * d3dpp.BackBufferHeight );
}

void D3DGraphics::EndFrame()
{
	HRESULT result;

	result = pBackBuffer->LockRect( &backRect,NULL,NULL );
	assert( !FAILED( result ) );

	for( unsigned int y = 0; y < d3dpp.BackBufferHeight; y++ )
	{
		memcpy( &((BYTE*)backRect.pBits)[ y * backRect.Pitch ], &pTestSurface[ y * d3dpp.BackBufferWidth ],sizeof( Color ) * d3dpp.BackBufferWidth );
	}

	result = pBackBuffer->UnlockRect();
	assert( !FAILED( result ) );

	result = pDevice->Present( NULL,NULL,NULL,NULL );
	assert( !FAILED( result ) );
}

void D3DGraphics::DrawDisc( int cx,int cy,int r, Color c )
{

	for( int x = cx - r; x < cx + r; x++ )
	{
		for( int y = cy - r; y < cy + r; y++ )
		{
			int dist = sqrt( (float)( (x - cx)*(x - cx) + (y - cy)*(y - cy) ) );

			if( dist < r )
			{
				PutPixel( x, y, c );
			}
		}
	}
}

void D3DGraphics::DrawDisc( Vec2& c,int r, float sf, Color color )
{
	int rad = (r * sf) + 0.5f;
	for( int x = c.x - rad; x < c.x + rad; x++ )
	{
		for( int y = c.y - rad; y < c.y + rad; y++ )
		{
			int dist = sqrt( (float)( (x - c.x)*(x - c.x) + (y - c.y)*(y - c.y) ) );

			if( dist < r )
			{
				PutPixel( x, y, color );
			}
		}
	}
}

void D3DGraphics::DrawLine( int x1,int y1,int x2,int y2, Color c )
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if( dy == 0 && dx == 0 )
	{
		PutPixel( x1,y1,c );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dx / (float)dy;
		float b = x1 - m*y1;
		for( int y = y1; y <= y2; y = y + 1 )
		{
			int x = (int)(m*y + b + 0.5f);
			PutPixel( x,y,c );
		}
	}
	else
	{
		if( dx < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		float m = (float)dy / (float)dx;
		float b = y1 - m*x1;
		for( int x = x1; x <= x2; x = x + 1 )
		{
			int y = (int)(m*x + b + 0.5f);
			PutPixel( x,y,c );
		}
	}
}

void D3DGraphics::DrawLine( Vec2& c1, Vec2& c2, Color c )
{
	int dx = c2.x - c1.x;
	int dy = c2.y - c1.y;

	if( dy == 0 && dx == 0 )
	{
		PutPixel( c1.x,c1.y,c );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0 )
		{
			int temp = c1.x;
			c1.x = c2.x;
			c2.x = temp;
			temp = c1.y;
			c1.y = c2.y;
			c2.y = temp;
		}
		float m = (float)dx / (float)dy;
		float b = c1.x - m*c1.y;
		for( int y = c1.y; y <= c2.y; y = y + 1 )
		{
			int x = (int)(m*y + b + 0.5f);
			PutPixel( x,y,c );
		}
	}
	else
	{
		if( dx < 0 )
		{
			int temp = c1.x;
			c1.x = c2.x;
			c2.x = temp;
			temp = c1.y;
			c1.y = c2.y;
			c2.y = temp;
		}
		float m = (float)dy / (float)dx;
		float b = c1.y - m*c1.x;
		for( int x = c1.x; x <= c2.x; x = x + 1 )
		{
			int y = (int)(m*x + b + 0.5f);
			PutPixel( x,y,c );
		}
	}
}

void D3DGraphics::DrawCircle( int centerX,int centerY,int radius, Color c )
{
	int rSquared = radius*radius;
	int xPivot = (int)(radius * 0.707107f + 0.5f);
	for( int x = 0; x <= xPivot; x++ )
	{
		int y = (int)(sqrt( (float)( rSquared - x*x ) ) + 0.5f);
		PutPixel( centerX + x, centerY + y, c );
		PutPixel( centerX - x, centerY + y, c );
		PutPixel( centerX + x, centerY - y, c );
		PutPixel( centerX - x, centerY - y, c );
		PutPixel( centerX + y, centerY + x, c );
		PutPixel( centerX - y, centerY + x, c );
		PutPixel( centerX + y, centerY - x, c );
		PutPixel( centerX - y, centerY - x, c );
	}
}

void D3DGraphics::DrawCircle( Vec2& c, int radius, float sf, Color color )
{
	int r = ( (float)radius * sf ) + 0.5f;
	int rSquared = r*r;
	int xPivot = (int)(radius * 0.707107f + 0.5f);
	for( int x = 0; x <= xPivot; x++ )
	{
		int y = (int)(sqrt( (float)( rSquared - x*x ) ) + 0.5f);
		PutPixel( c.x + x, c.y + y, color );
		PutPixel( c.x - x, c.y + y, color );
		PutPixel( c.x + x, c.y - y, color );
		PutPixel( c.x - x, c.y - y, color );
		PutPixel( c.x + y, c.y + x, color );
		PutPixel( c.x - y, c.y + x, color );
		PutPixel( c.x + y, c.y - x, color );
		PutPixel( c.x - y, c.y - x, color );
	}
}

void D3DGraphics::DrawSprite( int xoff,int yoff,Sprite* sprite )
{
	for( int y = 0; y < sprite->height; y++ )
	{
		for( int x = 0; x < sprite->width; x++ )
		{
			Color c = sprite->surface[ x + y * sprite->width ];
			if( c.A != sprite->key.A &&
				c.R != sprite->key.R &&
				c.G != sprite->key.G &&
				c.B != sprite->key.B )
			{
				PutPixel( x + xoff,y + yoff,c );
			}
		}
	}
}

void D3DGraphics::DrawSpriteAlpha( int xoff,int yoff,Sprite* sprite )
{
	for( int y = 0; y < sprite->height; y++ )
	{
		for( int x = 0; x < sprite->width; x++ )
		{
			PutPixel( x + xoff,y + yoff, sprite->surface[ x + y * sprite->width ]);
		}
	}
}

void D3DGraphics::DrawBGImg( int camx, int camy, Sprite* sprite)
{
	unsigned int xoff = camx - (d3dpp.BackBufferWidth / 2);
	unsigned int yoff = camy - (d3dpp.BackBufferHeight / 2);

	for( unsigned int y = 0; y < d3dpp.BackBufferHeight; y++ )
	{
		for( unsigned int x = 0; x < d3dpp.BackBufferWidth; x++ )
		{
			PutPixel( x, y, sprite->surface[ (x + xoff) + ((y + yoff) * sprite->width) ] );
		}
	}
}

void D3DGraphics::DrawChar( char c,int xoff,int yoff,Font* font,Color color )
{
	if( c < ' ' || c > '~' )
		return;

	const int sheetIndex = c - ' ';
	const int sheetCol = sheetIndex % font->nCharsPerRow;
	const int sheetRow = sheetIndex / font->nCharsPerRow;
	const int xStart = sheetCol * font->charWidth;
	const int yStart = sheetRow * font->charHeight;
	const int xEnd = xStart + font->charWidth;
	const int yEnd = yStart + font->charHeight;
	const int surfWidth = font->charWidth * font->nCharsPerRow;

	for( int y = yStart; y < yEnd; y++ )
	{
		for( int x = xStart; x < xEnd; x++ )
		{
			if( font->surface[ x + y * surfWidth ].A != 0 )
			{
				PutPixel( x + xoff - xStart,y + yoff - yStart, color );
			}
		}
	}
}

void D3DGraphics::DrawString( const char* string,int xoff,int yoff,Font* font,Color color )
{
	for( int index = 0; string[ index ] != '\0'; index++ )
	{
		DrawChar( string[ index ],xoff + index * font->charWidth,yoff,font,color );
	}
}

void D3DGraphics::DrawShield( unsigned int ShipX, unsigned int ShipY,
							  unsigned int shieldRadius, float shieldStrength )
{
	if(shieldStrength > 1.0f)
	{
		shieldStrength = 1.0f;
	}
	int div = 2;
	BYTE rd = (255 - (255 * shieldStrength)) / div;
	BYTE g = (255 * shieldStrength) / div;
	BYTE b = 0;


	//int nRad = -(shieldRadius);
	int nRad = shieldRadius;

	for( int x = -nRad; x < nRad; x++ )
	{
		for( int y = -nRad; y < nRad; y++ )
		{

			float dist = sqrt( (float)( (x)*(x) + (y)*(y) ) );

			if( dist < shieldRadius )
			{
				float shade = dist / shieldRadius;
				int shieldX = x + ShipX;
				int shieldY = y + ShipY;
				PutPixel( shieldX, shieldY, FILLCOLOR(shade * 255, rd, g, b) );
			}
		}
	}
}

void D3DGraphics::DrawShield( Vec2& pos, float shieldRadius, float shieldStrength, float sf )
{
	if(shieldStrength > 1.0f)
	{
		shieldStrength = 1.0f;
	}
	int div = 2;
	BYTE rd = (255 - (255 * shieldStrength)) / div;
	BYTE g = (255 * shieldStrength) / div;
	BYTE b = 0;


	//int nRad = -(shieldRadius);
	int nRadx = ( shieldRadius * sf ) + 0.5f;
	int nRady = ( shieldRadius * sf ) + 0.5f;

	for( int x = -nRadx; x < nRadx; x++ )
	{
		for( int y = -nRady; y < nRady; y++ )
		{

			float dist = sqrt( (float)( (x)*(x) + (y)*(y) ) );

			if( dist < shieldRadius )
			{
				float shade = dist / shieldRadius;
				int shieldX = x + pos.x;
				int shieldY = y + pos.y;
				PutPixel( shieldX, shieldY, FILLCOLOR(shade * 255, rd, g, b) );
			}
		}
	}
}

D3DGraphics::Color D3DGraphics::Blend(Color source1, Color source2)
{
	if( source1.A == 0 )
	{
		source1.A = 255;
	}
	Color result = FILLCOLOR_XRGB(((source1.R * ( (float)source1.A / (float)(source1.A + source2.A) * 255 ))
									+ source2.R * ((float)source2.A / (float)(source1.A + source2.A) * 255)) / 255,
								  ((source1.G * ( (float)source1.A / (float)(source1.A + source2.A) * 255 ))
									+ source2.G * ((float)source2.A / (float)(source1.A + source2.A) * 255)) / 255,
								  ((source1.B * ( (float)source1.A / (float)(source1.A + source2.A) * 255 ))
									+ source2.B * ((float)source2.A / (float)(source1.A + source2.A) * 255)) / 255);
	return result;
}

void D3DGraphics::DrawShip( int ShipX, int ShipY, int vx, int vy, int vx2, int vy2, Color c )
{
	DrawLine(ShipX + vx, ShipY + vy, ShipX + vx2, ShipY + vy2, c);
}

void D3DGraphics::DrawShip( Vec2& shipPos, Vec2& v1, Vec2& v2, Color color )
{
	DrawLine(shipPos + v1, shipPos + v2, color);
}

void D3DGraphics::DrawAmmo(float x, float y)
{
	Color c = FILLCOLOR(255, 255, 255, 255);
	for(int j = 0; j < 4; j++)
	{
		for(int i = 0; i < 2; i++)
		{
			PutPixel(x + i, y + j, c);
		}
	}
}

void D3DGraphics::DrawAmmo( Vec2& c, Vec2& sf )
{
	Color color = FILLCOLOR(255, 255, 255, 255);
	int ny = (4.0f * sf.y) + 0.5f;
	int nx = (4.0f * sf.x) + 0.5f;
	for(int j = 0; j < ny; j++)
	{
		for(int i = 0; i < nx; i++)
		{
			PutPixel(c + Vec2( i, j ), color);
		}
	}
		
}

void D3DGraphics::DrawCurve(int x1, int y1, int x2, int y2, int x3, int y3, Color c)
{
	int distX1 = abs(x3 - x2);
	int distX2 = abs(x2 - x1);
	int distY1 = abs(y3 - y2);
	int distY2 = abs(y2 - y1);

	float SideA = sqrt((float)(distX1 ^ 2) + (float)(distY1 ^ 2));
	float SideB = sqrt((float)(distX2 ^ 2) + (float)(distY2 ^ 2));
	float SideC = sqrt((pow(SideA, 2) + pow(SideB, 2)));

	float sine = SideA / SideC;
	float sinA = sin(45 * (3.14159f / 180));
	unsigned int step = 45;
	unsigned int turns = 360 / step;
	float *x = new float [turns];
	float *y = new float [turns];
	float PI = 3.14159f;
	float Rad = PI / 180;
	unsigned int radius = 200;
	unsigned int cx = 0;
	unsigned int cy = 0;
	unsigned int cx2 = 0;
	unsigned int cy2 = 0;
	unsigned int j = 0;

	for(unsigned int i = 0; i < 360; i += step)
	{
		x[i / step] = (radius * sin(i * Rad)) + 0.5f;
		y[i / step] = (radius * cos(i * Rad)) + 0.5f;
		
	}
	for( unsigned int i = 0; i < turns; i++ )
	{

		j = i + 1;
		if(j == turns)
		{
			j = 0;
		}
		cx = x[i] + 400;
		cy = y[i] + 300;
		cx2 = x[j] + 400;
		cy2 = y[j] + 300;
		DrawLine(cx, cy, cx2, cy2, c);
	}
	delete x;
	delete y;
}

D3DGraphics::Color D3DGraphics::FILLCOLOR(BYTE a, BYTE r, BYTE g, BYTE b)
{
	Color c = {b, g, r, a};
	return c;
}

D3DGraphics::Color D3DGraphics::FILLCOLOR_XRGB(BYTE r, BYTE g, BYTE b)
{
	Color c = { b, g, r, 255 };
	return c;
}

void D3DGraphics::DrawButton( int x, int y, int x2, int y2, const char* text, Font* font,Color c )
{
	DrawLine( x, y, x2, y, c );
	DrawLine( x, y2, x2, y2, c );
	DrawLine( x2, y, x2, y2, c );
	DrawLine( x, y, x, y2, c );
	DrawString(text, x + ((x2 - x) - (5 * font->charWidth)) / 2, y + ((y2 - y) - (font->charHeight) ) / 2, font, c);
}

void D3DGraphics::LoadSpriteAlpha( Sprite* sprite, LPCTSTR filename, Color key )
{
	Color surfColor = {0};
	LPCTSTR pFile = filename;
	Gdiplus::Bitmap bitmap( pFile );
	Gdiplus::Color pixel;

	sprite->height = bitmap.GetHeight();
	sprite->width = bitmap.GetWidth();
	sprite->key = FILLCOLOR(255, 255, 255, 255);

	sprite->surface = new Color[ sprite->height * sprite->width ];

	for( unsigned int y = 0; y < bitmap.GetHeight(); y++ )
	{
		for( unsigned int x = 0; x < bitmap.GetWidth(); x++ )
		{
			bitmap.GetPixel( x,y,&pixel );

			surfColor = FILLCOLOR( pixel.GetA(), pixel.GetR(), pixel.GetG(), pixel.GetB() );
			if(surfColor.A == key.A &&
			   surfColor.R == key.R &&
			   surfColor.G == key.G &&
			   surfColor.B == key.B )
			{
				surfColor.A = 0;
			}
				sprite->surface[ x + y * sprite->width ] = surfColor;

		}
	}
}

void D3DGraphics::FreeSprite( Sprite* sprite )
{
	delete [] sprite;
}

void D3DGraphics::LoadFont( Font* font,Color* surface,LPCTSTR filename,
	int charWidth,int charHeight,int nCharsPerRow )
{

	Sprite Font;
	Color key = FILLCOLOR(255, 255, 255, 255);
	LoadSpriteAlpha(&Font, filename, key);
	font->charHeight = Font.height / 3;
	font->charWidth = Font.width / nCharsPerRow;
	font->nCharsPerRow = nCharsPerRow;
	font->surface = Font.surface;
}

// Particle functions
//void D3DGraphics::DrawBurner( int x, int y, int angle )
//{
//	if( !particleEmitter.EmitterCreated() )
//	{
//		particleEmitter.CreateEmitter(x, y, 10, 100, 0.05f, angle, 1, 1.0f);
//	}
//
//	particleEmitter.UpdateEmitterPosition(x, y);
//	particleEmitter.UpdateEmitterOrientation(angle);
//
//}
