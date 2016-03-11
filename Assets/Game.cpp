#include "Game.h"

Game::Game( HWND hWnd )
:	gfx( hWnd ),
	audio( hWnd ),
	sc( core ),
	core(gfx, audio)
{
	// Init random generator
	srand( (unsigned int)time( NULL ) );
	core.ic.SetClip(Vec2(gfx.GetWidth(), gfx.GetHeight()));
}

Game::~Game()
{
}

void Game::Go()
{	
	UpdateTimer();
	core.ic.Update();
	sc.Do();
	gfx.EndFrame();
}

void Game::UpdateTimer()
{
	TimerServer::Update();
}

//void Game::DoState( Game::GameState State )
//{	
//		// This case will be used to test drawing routines
//		gfx.BeginFrame();
//
//		if(!gfx.particleEmitter.EmitterCreated() )
//		{
//			gfx.particleEmitter.CreateEmitter(399, 0, SCREENWIDTH - 1, 100, 0.1f, 180, rand() % 5, 3.0);
//		}
//		gfx.particleEmitter.UpdateTimers();
//		gfx.particleEmitter.LaunchParticle();
//		gfx.particleEmitter.UpdateParticles();
//		for(int i = 0; i < gfx.particleEmitter.Size(); i++)
//		{
//			int x = gfx.particleEmitter.GetParticleX(i);
//			int y = gfx.particleEmitter.GetParticleY(i);
//			gfx.DrawAmmo(x, y);
//		}
//}

void Game::ComposeFrame()
{
}