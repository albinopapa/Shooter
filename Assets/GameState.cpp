#include "GameState.h"
#include "Menu.h"
#include "ScoreController.h"
#include "Shop.h"
GameState::GameState(StateController &sc, StateCore &core )
	:
	sc(sc),
	core(core),
	gfx(core.gfx),
	ic(core.ic)
{
	f.surface = std::make_unique<D3DGraphics::Color[]>(512 * 84);
	gfx.LoadFont(&f, f.surface.get(), L"Fonts\\Fixedsys16x28.bmp", 16, 28, 32);
}

GameState::~GameState()
{
}
