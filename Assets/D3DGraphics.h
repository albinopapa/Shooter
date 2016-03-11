#pragma once
#include <d3d9.h>
#include "Vec2.h"

#define SCREENWIDTH 800
#define SCREENHEIGHT 600
#define FILLVALUE 0xFF000000



class D3DGraphics
{
public:
	struct Color
	{
		BYTE B, G, R, A;
	};

	struct VFONT
	{
		Vec2 *pPoint;
		unsigned char nPoints;
	};

	struct Sprite
	{
		int width;
		int height;
		Color key;
		Color* surface;
		IDirect3DSurface9* pd3dSurf;
		IDirect3DTexture9* pd3dSTex;

	};

	struct Font
	{
		int charWidth;
		int charHeight;
		int nCharsPerRow;
		Color* surface;
	};


	D3DGraphics( HWND hWnd );
	~D3DGraphics();
	void BeginFrame();
	void PutPixel( unsigned int x,unsigned int y, Color c );
	void PutPixel( Vec2& c, Color color );
	bool IsWindowed() const;
	HWND GetWindow() const;
	unsigned GetWidth() const;
	unsigned GetHeight() const;
	Color GetPixel( int x,int y ) const;
	bool Reset();
	void ToggleMode();
	void LoadSpriteAlpha( Sprite* sprite, LPCTSTR filename, Color key );
	void FreeSprite( Sprite* sprite );
	void LoadFont( Font* font,Color* surface,LPCTSTR filename,
		int charWidth,int charHeight,int nCharsPerRow );

	void LoadVFont();

	void DrawLine( int x1,int y1,int x2,int y2, Color c );
	void DrawLine( Vec2& c1, Vec2& c2, Color c );
	void DrawCurve( int x1, int y1, int x2, int y2, int x3, int y3, Color c );
	void DrawCircle( int cx,int cy,int radius, Color c );
	void DrawCircle( Vec2& c,int radius, float sf, Color color );

	void DrawDisc( int cx,int cy,int r, Color c );
	void DrawDisc( Vec2& c,int r, float sf, Color color );
	void DrawSprite( int x,int y,Sprite* sprite );
	void DrawSpriteAlpha( int xoff,int yoff,Sprite* sprite );
	void DrawChar( char c,int x,int y,Font* font, Color clr );
	void DrawString( const char* string,int x,int y,Font* font, Color c );
	void DrawBGImg( int x, int y, Sprite* sprite );
	void DrawAmmo( float x, float y );
	void DrawAmmo( Vec2& c, Vec2& sf );
	void DrawLetter( char letter );
	
	void DrawShield( unsigned int ShipX, unsigned int ShipY, unsigned int shieldRadius, float shieldStrength );
	void DrawShield( Vec2& pos, float shieldRadius, float shieldStrength, float sf );
	void DrawShip( int ShipX, int ShipY, int vx, int vy, int vx2, int vy2, Color c );
	void DrawShip( Vec2& shipPos, Vec2& v1, Vec2& v2, Color color );
	Color Blend( Color source1, Color source2 );
	Color FILLCOLOR(BYTE a, BYTE r, BYTE g, BYTE b);
	Color FILLCOLOR_XRGB(BYTE r, BYTE g, BYTE b);
	void DrawButton( int x, int y, int x2, int y2, const char* text, Font* font, Color c );
	// Future functions for particles
	// ------------------------------
	//void DrawBurner( int x, int y, int angle );
	// ------------------------------
	void EndFrame();

private:
	IDirect3D9*			pDirect3D;
	IDirect3DDevice9*	pDevice;
	IDirect3DSurface9*	pBackBuffer;
	D3DLOCKED_RECT		backRect;
	D3DPRESENT_PARAMETERS d3dpp;

	IDirect3DTexture9*	pD3DTexture;
	IDirect3DSurface9*	pD3DSurface;
	
	Color*				pTestSurface;
	Sprite				bgImage;
	VFONT				*vFont;
	// Test

};
