#include "WeaponController.h"
#include <new>
#include "EntityController.h"
#include "StateCore.h"

WeaponController::WeaponController(StateCore &core)
	:
	core(core)	
{
	weapon = new Weapon*[nWeps];
	weapon[0] = new Blaster(core, true, true, 1, "Blaster", 500);
	weapon[1] = new MachineGun(core, false, false, 0, "Machine Gun", 500);
	weapon[2] = new MLauncher(core, false, false, 0, "Missile Launcher", 500);
	weapon[3] = new ECannon(core, false, false, 0, "Energy Cannon", 500);
	weapon[4] = new LaserWeap(core, true, true, 5, "Lazer", 500);
	hotkeys[ 0 ] = DIK_1;
	hotkeys[ 1 ] = DIK_2;
	hotkeys[ 2 ] = DIK_3;
	hotkeys[ 3 ] = DIK_4;
	hotkeys[ 4 ] = DIK_5;	
	curWeapon = weapon[4];
	Energy = 100;
	RechargeRate = 1.0f;
}

void WeaponController::Fire(const Vec2 &pos)
{
	TimerClient tc;
	if(Energy >= curWeapon->EnergyDrain * tc.GetFrameTime() )
	{
		curWeapon->Fire(pos);
		ModifyEnergy( curWeapon->EnergyDrain );
		curWeapon->isFiring = true;
	}
}

bool WeaponController::Has(Weapon *wep)
{	
	return wep->have;
}

void WeaponController::SwitchWeapon()
{
	// 0 = Blaster, 1 = MachineGun, 2 = Launcher, 3 = MachineGun
	for(int i = 0; i < nWeps; i++)
	{
		if( core.ic.KeyTyped( hotkeys[ i ] ) )
		{
			if( weapon[i]->unlocked )
			{
				curWeapon = weapon[i];
			}
		}
	}
}

unsigned int WeaponController::GetCurrentUpgrade(Weapon *wep)
{
	return wep->curlevel;
}

bool WeaponController::IsUnlocked(Weapon *wep)
{
	return wep->have;
}

void WeaponController::Upgrade(Weapon *wep)
{
	wep->nextLevel = wep->curlevel + 1;
	if(wep->nextLevel < nUpgrades)
	{
		wep->curlevel++;
	}
}

float WeaponController::GetEnergy()
{
	return Energy;
}

void WeaponController::ModifyEnergy( float amount )
{
	TimerClient tc;
	if(Energy > 0.0f)
	{
		Energy -= amount * curWeapon->curlevel * tc.GetFrameTime();
	}
}

void WeaponController::RechargeEnergy()
{
	TimerClient tc;
	if(Energy < 100.0f)
	{
		Energy += RechargeRate * curWeapon->curlevel * tc.GetFrameTime();
	}
}

void WeaponController::Downgrade(Weapon *wep)
{
	wep->curlevel--;
}

void WeaponController::Lock(Weapon *wep)
{
	wep->unlocked = false;
}

void WeaponController::Unlock(Weapon *wep)
{
	wep->unlocked = true;
}

int WeaponController::PurchasePrice(Weapon *wep)
{
	return -(wep->nextLevel * 500);
}

int WeaponController::SalePrice(Weapon *wep)
{
	return wep->curlevel * 500;
}

Weapon *WeaponController::GetWeapon( unsigned int i)
{
	return weapon[ i ];
}

WeaponController::~WeaponController()
{
}
