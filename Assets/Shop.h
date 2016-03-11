#pragma once

#include <string>

#include "GameState.h"
#include "Button.h"
#include "WeaponController.h"
#include "ScoreController.h"

#define nButtons 5

class Inventory
{
public:
	Inventory(std::string Name, Vec2 pos, unsigned int cost, D3DGraphics &gfx, Weapon *w);
	void Draw(D3DGraphics &gfx, D3DGraphics::Font *font, D3DGraphics::Color color);
	Button buy;
	Button sell;
	std::string str;
	Weapon *wep;
	unsigned int cost, X, Y;
	
};

class WepPur
{
public:
	WepPur();
	WepPur(StateCore &core);
	~WepPur();
	void Unlock(Weapon *wep); //for use in levels when the upgrade can be purchased
	void Upgrade(Weapon *wep);
	unsigned int GetPoints();
	void ModifyPoints(int amount);
	void Do(Inventory &inv);
private:
	StateCore &core;
};

class Shop : public GameState
{
public:
	Shop( StateController &sc, StateCore &core);
	~Shop();	
	void Do()override;
	void BeginFrame()override
	{
		gfx.BeginFrame();
	}
	void ComposeFrame()override;

private:
	void Draw();
	void readData();
	void writeData();

private:
	std::unique_ptr<WepPur> weppur;
	std::string filename;
	std::unique_ptr<std::unique_ptr<Inventory>[]> inv;

};
