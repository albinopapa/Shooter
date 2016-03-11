#pragma once
#include "D3DGraphics.h"
#include <string>
#include <gdiplus.h>
#include "Game.h"

class Surface
{
public:
	Surface()
	{
		
	};

	~Surface()
	{
		
	};
	void LoadSurface(std::wstring &filename)
	{
		Gdiplus::Bitmap bitmap( filename.c_str() );
		Gdiplus::Color pixel;
		D3DGraphics::Color dxPixel;
		height = bitmap.GetHeight();
		width = bitmap.GetWidth();
		surface = new D3DGraphics::Color [height * width ];

		for( unsigned int y = 0; y < bitmap.GetHeight(); y++ )
		{
			for( unsigned int x = 0; x < bitmap.GetWidth(); x++ )
			{
				bitmap.GetPixel( x,y,&pixel );
				dxPixel.A = pixel.GetA();
				dxPixel.R = pixel.GetR();
				dxPixel.G = pixel.GetG();
				dxPixel.B = pixel.GetB();
				surface[ x + y * width] = dxPixel;
			}
		}
	}
	
	void delSurface()
	{
		delete [] surface;
	}

	void Draw(int xoff, int yoff,D3DGraphics& gfx)
	{
		const  int yStart = max(-yoff,0);
		const  int xStart = max(-xoff,0);
		const  int yEnd = min(SCREENHEIGHT - yoff,(int) height + 1 );
		const  int xEnd = min(SCREENWIDTH - xoff, (int) width - 1 );

		for(  int y = yStart; y < yEnd; y++ )
		{
			for(  int x = xStart; x < xEnd; x++ )
			{
				gfx.PutPixel( x + xoff,y + yoff,surface[ x + y * width ] );
			}
		}
	}
	void DrawKeyed(int xoff, int yoff, D3DGraphics::Color key,D3DGraphics& gfx)
	{
		const int yStart = max(-yoff,0);
		const int xStart = max(-xoff,0);
		const int yEnd = min(SCREENHEIGHT - yoff, (int)height );
		const int xEnd = min(SCREENWIDTH - xoff, (int)width );

		for( int y = yStart; y < yEnd; y++ )
		{
			for( int x = xStart; x < xEnd; x++ )
			{
				D3DGraphics::Color c = surface[ x + y * width ];
				if (c.A != key.A )
				{
					gfx.PutPixel( x + xoff,y + yoff,c);
				}
			}
		}
	}
	//void DrawAniKeyed
	/*void aniKeyed( Game::Action, Game::Direction direction, int frame, int x, int y )
	{
		
	}*/

private:
	unsigned int width;
	unsigned int height;
	D3DGraphics::Color* surface;
};
