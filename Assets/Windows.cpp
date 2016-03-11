/******************************************************************************************
 *	Chili DirectX Framework Version 12.10.21											  *
 *	Windows.cpp																			  *
 *	Copyright 2012 PlanetChili.net														  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include <Windows.h>
#include <WindowsX.h>
#include <wchar.h>
#include <GdiPlus.h>
#include "Game.h"
#include "resource.h"
#pragma comment( lib,"gdiplus.lib" )
Game *game = 0;
HWND hWnd = NULL;

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_CLOSE:
			PostQuitMessage( 0 );
			break;
		case WM_SIZE:
			if( game )
				//game->OnSize();
			break;
		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}


int WINAPI wWinMain( HINSTANCE hInst,HINSTANCE,LPWSTR,INT )
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	Gdiplus::GdiplusStartup( &gdiplusToken,&gdiplusStartupInput,NULL );

	WNDCLASSEX wc = { sizeof( WNDCLASSEX ),CS_CLASSDC,MsgProc,0,0,
					  GetModuleHandle( NULL ),NULL,NULL,NULL,NULL,
					  L"Chili DirectX Framework Window",NULL };
	wc.hIconSm = (HICON)LoadImage( hInst,MAKEINTRESOURCE( IDI_APPICON16 ),IMAGE_ICON,16,16,0 );
	wc.hIcon   = (HICON)LoadImage( hInst,MAKEINTRESOURCE( IDI_APPICON32 ),IMAGE_ICON,32,32,0 );
	wc.hCursor = LoadCursor( NULL,IDC_ARROW );
	RegisterClassEx( &wc );

	RECT wr;
	wr.left = 300;
	wr.right = SCREENWIDTH + wr.left;
	wr.top = 150;
	wr.bottom = SCREENHEIGHT + wr.top;
	AdjustWindowRect( &wr,WS_OVERLAPPEDWINDOW,FALSE );
	hWnd = CreateWindowW( L"Chili DirectX Framework Window",L"Chili DirectX Framework",
							  WS_OVERLAPPEDWINDOW,wr.left,wr.top,wr.right-wr.left,wr.bottom-wr.top,
							  NULL,NULL,wc.hInstance,NULL );

	ShowWindow( hWnd,SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	Game theGame( hWnd );
	game = &theGame;

	MSG msg;
	ZeroMemory( &msg,sizeof( msg ) );
	
	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg,NULL,0,0,PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			theGame.Go();			

			wchar_t sz[1024];
			static int fps=1, lfps=0;
			float t = (float)timeGetTime();
			static float lt = t;
			if( t - lt >= 1000 ) 
			{
				fps = lfps;
				lfps = 0;
				lt = t;
			}
			wsprintf( sz, L"Chili DirectX Framework - %d fps", fps ), lfps++;
			SetWindowText( hWnd, sz );
		}
	}

	UnregisterClass( L"Chili DirectX Framework Window",wc.hInstance );
	Gdiplus::GdiplusShutdown( gdiplusToken );
	return 0;
}
