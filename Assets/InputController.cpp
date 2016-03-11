#include "InputController.h"

InputController::InputController( D3DGraphics &gfx )
	:
	gfx(gfx),
	mPos(gfx.GetWidth() / 2,gfx.GetHeight() / 2),
	mouse(gfx.GetWindow()),
	kbd(gfx.GetWindow() ),
	jStick(gfx.GetWindow() ),
	paused(false),
	mBounds(Vec2(gfx.GetWidth(), gfx.GetHeight() ) ),
	jPos(Vec2(0, 0)),
	mCLF(false),
	kbCLF(false)
{
}

InputController::~InputController()
{
	
}

void InputController::Update()
{
	if(mouse.Check())
	{
		mouse.Update();
		float mx = static_cast<float>(mouse.GetX());
		float my = static_cast<float>(mouse.GetY());
		mPos += {mx, my};
		ClipMouse();
	}
	if(kbd.Check() )
	{
		kbd.Update();
	}
	if(jStick.Check() )
	{
		jStick.Update();
		jPos = Vec2((jStick.GetX() - 32767) / 32767, (jStick.GetY() - 32767) / 32767);
	}	
}


//Mouse functions
void InputController::SetClip(const Vec2 &mclip)
{
	mBounds = mclip;
}

Vec2 InputController::GetMousePos()
{
	return mPos;
}

void InputController::ClipMouse()
{
	if(mPos.x < 0)
	{
		mPos.x = 0;
	}
	else if(mPos.x >= mBounds.x)
	{
		mPos.x = mBounds.x - 1;
	}
	if(mPos.y < 0)
	{
		mPos.y = 0;
	}
	else if( mPos.y >= mBounds.y)
	{
		mPos.y = mBounds.y - 1;
	}
}

//Joystick functions
void InputController::GetAxes(float *x, float *y)
{
	Vec2 temp;
	if( jStick.Check() )
	{
		temp = jPos;
	}
	// W or UpArrow
	if( kbd.GetButtons( DIK_W ) || kbd.GetButtons( DIK_UP ) )
	{
		*y = -1.0f;
	}
	// S or DownArrow
	else if( kbd.GetButtons( DIK_S ) || kbd.GetButtons( DIK_DOWN ) )
	{
		*y = 1.0f;
	}
	else
	{
		*y = temp.y;
	}
	// A or LeftArrow
	if( kbd.GetButtons( DIK_A ) || kbd.GetButtons( DIK_LEFT ) )
	{
		*x = -1.0f;
	}
	// D or RightArrow
	else if( kbd.GetButtons( DIK_D ) || kbd.GetButtons( DIK_RIGHT ) )
	{
		*x = 1.0f;
	}
	else
	{
		*x = temp.x;
	}
}

Vec2 InputController::GetAxes()
{
	Vec2 temp, result;
	if (jStick.Check())
	{
		temp = jPos;
	}
	// W or UpArrow
	if (kbd.GetButtons(DIK_W) || kbd.GetButtons(DIK_UP))
	{
		result.y = -1.0f;
	}
	// S or DownArrow
	else if (kbd.GetButtons(DIK_S) || kbd.GetButtons(DIK_DOWN))
	{
		result.y = 1.0f;
	}
	else
	{
		result.y = temp.y;
	}
	// A or LeftArrow
	if (kbd.GetButtons(DIK_A) || kbd.GetButtons(DIK_LEFT))
	{
		result.x = -1.0f;
	}
	// D or RightArrow
	else if (kbd.GetButtons(DIK_D) || kbd.GetButtons(DIK_RIGHT))
	{
		result.x = 1.0f;
	}
	else
	{
		result.x = temp.x;
	}

	return result;
}

bool InputController::Firing()
{	
	// Fire = Spacebar, Left Mouse Button, Joystick button 0
	bool fire = false;
	if(kbd.Check())
	{
		if( kbd.GetButtons( DIK_SPACE) )
		{
			fire = true;
		}
	}
	if(mouse.Check() )
	{
		if( mouse.GetButtons( DIMOUSE_BUTTON0 -12 ) )
		{
			fire = true;
		}
	}
	if(jStick.Check() )
	{
		if( jStick.GetButtons( DIJOFS_BUTTON0 - DIJOFS_BUTTON0 ) )
		{
			fire = true;
		}
	}
	return fire;
}

bool InputController::IsPaused()
{
	return paused;
}

bool InputController::LeftButtonClicked()
{
	if(mCLF && !mouse.GetButtons( 0 ))
	{
		mCLF = false;
		return true;
	}
	else if( !mCLF && mouse.GetButtons( 0 ) )
	{
		mCLF = true;
	}
	return false;
}

bool InputController::KeyTyped( unsigned char code)
{
	if(kbCLF && !kbd.GetButtons(code) )
	{
		kbCLF = false;
	}
	else if(!kbCLF && kbd.GetButtons(code) )
	{
		kbCLF = true;
		return true;
	}
	return false;
}

void InputController::ClipAxis(unsigned int maxClipSize, float* axis)
{
	if(*axis < 0)
	{
		*axis = 0;
	}
	if(*axis >= maxClipSize)
	{
		*axis = maxClipSize - 1;
	}
}

