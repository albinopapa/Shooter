#include "Button.h"

Button::Button()
{}

Button::Button(D3DGraphics::Color color, unsigned int x, unsigned int y, 
		unsigned int width, unsigned int height, const std::string &text)
		:
		X(x),
		Y(y),
		X2( x + width), 
		Y2( y + height ), 
		Text(text), 
		c(color)
		{}

bool Button::ButtonClicked(InputController &ic, D3DGraphics &gfx )
{
	Vec2 mPos = ic.GetMousePos();
	bool clicked = false;
	c = gfx.FILLCOLOR_XRGB(255, 255, 255);
	if( (X < mPos.x && mPos.x < X2) && (Y < mPos.y && mPos.y < Y2 ))
	{
		c = gfx.FILLCOLOR_XRGB(255, 0, 255);
		clicked = ic.LeftButtonClicked();
	}
	return clicked;
}

const char*Button::GetText()const
{
	return Text.c_str();
}

void Button::Draw(D3DGraphics::Font &Font, D3DGraphics &gfx)
{
	gfx.DrawButton( X, Y, X2, Y2, Text.c_str(), &Font, c);
}
