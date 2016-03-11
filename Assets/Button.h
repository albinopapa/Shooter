#pragma once
#include <string>
#include "D3DGraphics.h"
#include "InputController.h"
class Button
{
public:	
	Button();
	Button(D3DGraphics::Color color, unsigned int x, unsigned int y, 
		unsigned int width, unsigned int height, const std::string &text);
	void Draw(D3DGraphics &gfx);
	bool ButtonClicked(InputController &ic, D3DGraphics &gfx );
	const char*GetText()const;
private:
	std::string Text;
	unsigned int X, Y, X2, Y2;
	D3DGraphics::Color c;
	D3DGraphics::Font font;
};