#pragma once

#include <vector>
#include "InputController.h"
#include "Vec2.h"
#include "Weapon.h"

class StateCore;

/* This class is to switch weapons and act as a bridge between the different states*/
class WeaponController
{
public:
	WeaponController(StateCore &core);
	~WeaponController();

	// Player wc functions
	void Fire(const Vec2 &Pos);
	void SwitchWeapon();
	float GetEnergy();
	void ModifyEnergy( float amount );
	void RechargeEnergy();

	// Shop wc functions
	void Give(bool isGetting, Weapon *wep);
	bool IsUnlocked(Weapon *wep);
	bool Has(Weapon *wep);
	unsigned int GetCurrentUpgrade(Weapon *wep);
	std::string GetName(Weapon *wep);
	Weapon *GetWeapon(unsigned int i);
	void Upgrade(Weapon *wep);
	void Downgrade(Weapon *wep);
	void Lock(Weapon *wep);
	void Unlock(Weapon *wep);
	int PurchasePrice(Weapon *wep);
	int SalePrice(Weapon *wep);
private:
	float Energy, RechargeRate;
	Weapon **weapon;
	Weapon *curWeapon;
	char hotkeys[ nWeps ];
	StateCore &core;

};
