#include "Shop.h"
#include "WeaponController.h"
/*
Purpose of Shop class is the ic interface for purchasing and selling Weapons and
weapon upgrades.  It's role is to display a list of guns and depending on what 
the player has unlocked, allow the ic to click on buttons to purchase, upgrade 
or sell the gun.
*/


Shop::Shop( StateController &sc, StateCore &core)
	:
	GameState(sc, core),
	filename( "Shop.txt" ),
	weppur(std::make_unique<WepPur>(core))
{	 
	int bX = 25;
	int bY = 50;
	int bWidth = 100;
	int bHeight = 50;
	int bSpacing = 40;
	int next = bHeight + bSpacing;
	inv = std::make_unique<std::unique_ptr<Inventory>[]>(nWeps);

	Vec2 pos(static_cast<float>(bX), static_cast<float>(bY));
	unsigned int i = 0;
	WeaponController *wc = core.player.GetWC();
	for(unsigned int i = 0; i < nWeps; i++)
	{
		Weapon* wep = wc->GetWeapon(i);
		switch (i)
		{
		case 0:
			inv[i] = std::make_unique<Inventory>("Blaster", pos, wc->GetCurrentUpgrade(wep) * 500, core.gfx, wep);
			break;
		case 1:
			inv[i] = std::make_unique<Inventory>("M. Gun", pos, wc->GetCurrentUpgrade(wep) * 500, core.gfx, wep);
			break;
		case 2:
			inv[i] = std::make_unique<Inventory>("Missile", pos, wc->GetCurrentUpgrade(wep) * 500, core.gfx, wep);
			break;
		case 3:
			inv[i] = std::make_unique<Inventory>("E.Cannon", pos, wc->GetCurrentUpgrade(wep) * 500, core.gfx, wep);
			break;
		case 4:
			inv[i] = std::make_unique<Inventory>("Lazer", pos, wc->GetCurrentUpgrade(wep) * 500, core.gfx, wep);
			break;
		}
		pos.y += next;
	}
}

void Shop::Do()
{
	BeginFrame();
	for(unsigned int i = 0; i < nButtons; i++)
	{
		weppur->Do( *inv[i] );
	}
	ComposeFrame();
	
}

void Shop::ComposeFrame()
{
	for( unsigned int i = 0; i < nButtons; i++)
	{
		inv[ i ]->Draw(gfx, &f, gfx.FILLCOLOR_XRGB(255, 255, 255) );
	}	
}

void Shop::readData()
{
	//for( int index = 0; index < objMax; index++ )
	//{
	//	fseek( pfile, std::strlen(button[ index ].GetText()), SEEK_CUR );
	//	fscanf( pfile, "%f\n", curUnlocks );
	//}
	//fscanf( pfile, "%f\t%f", points, curUnlocks );
	////weppur = new WepPur[ curUnlocks ];
	//for( unsigned int i = 0; i < curUnlocks; i++ )
	//{
	//	wep[ i ].unlocked = true;
	//}
}

void Shop::writeData()
{
	/*pfile = fopen( filename.c_str(), "w" );
	for( int index = 0; index < objMax; index++ )
	{
		fprintf( pfile, "%s\t%f\n", wep[ index ].name, wep[ index ].level );
	}
	fprintf( pfile, "%f\t%f", points, curUnlocks );
	fclose( pfile );*/
}

Shop::~Shop()
{
}

// Weapon purchase class
WepPur::WepPur( StateCore &core)
	:
	core(core)
{
}

WepPur::~WepPur()
{
}

void WepPur::Do( Inventory &inv )
{
	WeaponController *wc = core.player.GetWC();

	if(inv.buy.ButtonClicked(core.ic, core.gfx))
	{
		if(!wc->IsUnlocked(inv.wep))
		{
			if( GetPoints() >= (unsigned)abs(wc->PurchasePrice(inv.wep)) )
			{
				ModifyPoints( wc->PurchasePrice(inv.wep) );
				Unlock( inv.wep );
			}
		}
		else
		{
			if(GetPoints() >= (unsigned)abs(wc->PurchasePrice(inv.wep)) )
			{
				ModifyPoints( wc->PurchasePrice(inv.wep) );
				Upgrade( inv.wep );
			}
		}
	}
	else if(inv.sell.ButtonClicked(core.ic, core.gfx))
	{
		if(wc->Has(inv.wep))
		{
			ModifyPoints( wc->SalePrice(inv.wep) );
			wc->Downgrade(inv.wep);
		}		
	}
}

unsigned int WepPur::GetPoints()
{	
	return core.cScore.GetPlayerScore();
}

void WepPur::ModifyPoints(int amount)
{
	core.cScore.Add(amount);
}

void WepPur::Upgrade(Weapon *wep)
{
	WeaponController *wc = core.player.GetWC();
	wc->Upgrade(wep);
}

void WepPur::Unlock(Weapon *wep)
{
	WeaponController *wc = core.player.GetWC();
	wc->Unlock(wep);
}

Inventory::Inventory(std::string Name, Vec2 pos, unsigned int Cost, D3DGraphics &gfx, Weapon *w)
	:
	str(Name),
	cost(Cost),
	wep(w),
	X(pos.x),
	Y(pos.y)
{
	D3DGraphics::Color color = gfx.FILLCOLOR_XRGB(255, 255, 255);

	buy = Button(color, X, Y, 100, 50, "Buy");
	sell = Button(color, X + 125, Y, 100, 50, "Sell");
}

void Inventory::Draw(D3DGraphics &gfx, D3DGraphics::Font *font, D3DGraphics::Color color)
{
	gfx.DrawString(str.c_str(), X, Y - 30, font, color);
	buy.Draw(*font, gfx);
	sell.Draw(*font, gfx);
}