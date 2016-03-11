#include "Play.h"
#include "ScoreController.h"
#include "EntityController.h"
#include <typeinfo.h>
Play::Play(StateController &sc, StateCore &core)
:
GameState(sc, core),
ec(core),
player(&core.player),
cam(core.level->GetCam())
{
	l = sc.GetLevel();	
	core.gameStarted = true;
}

void Play::BeginFrame()
{
	gfx.BeginFrame();
}

void Play::Do()
{
	/*
	Order of commands	
	- Update camera position
	- Update player
		- Check InputController for fire button pressed
		- Update player position
		- Update shield position
	- Update ammo
		- Update ammo position
	- Check Triggers
		- Spawn entities using tirgger locations as spawn points and type
	- Update enemies
		- Update enemy position
		- Update player position with enemy
		- Check for collision
			- Check distance between objects
			- Call entities' DoCollision function if hit
				- Update health
		- Delete dead entities
			- Remove entities with health at or below 0
			- Update player score
				- Add to player score if enemy was killed by player ammo
	- Update asteroids
		- Update position
		- Check for collision
			- Check distance between objects
			- Call entities' DoCollision function if hit
				- Update health
		- Delete dead entities
			- Remove entities with health at or below 0
			- Update player score
				- Add to player score if enemy was killed by player ammo
	*/
	BeginFrame();

	CheckInput();
	CheckTriggers();
	
	UpdateCamera();
	UpdatePlayer();
	UpdateAmmo();
	UpdateEnemies();
	UpdateAsteroids();
	UpdateBoss();

	CheckCollisions();
	CleanVectors();
	
	ComposeFrame();
}

void Play::CheckInput()
{
	if(core.ic.KeyTyped(DIK_ESCAPE))
	{
		sc.DoEscape();
	}

	if (core.ic.KeyTyped(DIK_F10))
	{
		sc.DoF10();
	}
}

void Play::CheckTriggers()
{
	l->CheckTriggers();
}

void Play::UpdatePlayer()
{
	ec.Update(player);
}

void Play::UpdateCamera()
{
	cam->Do();
}

void Play::UpdateEnemies()
{
	Vec2 pPos(core.player.GetC());
	for (unsigned int i = 0; i < core.enemy.size(); i++)
	{
		core.enemy[ i ]->RevealPlayerPosition( pPos );
		ec.Update(core.enemy[i].get());
		if (ec.IsDead(core.enemy[i].get()))
		{
			if (core.enemy[i]->GotHitByAmmo())
			{
				core.cScore.Add(core.enemy[i]->GetValue());
			}
		}
	}
}

void Play::UpdateAsteroids()
{
	for (unsigned int i = 0; i < core.asteroid.size(); i++)
	{
		ec.Update(core.asteroid[i].get());
	}
}

void Play::UpdateAmmo()
{
	for(unsigned int i = 0; i < core.ammo.size(); i++)
	{		
		ec.Update(core.ammo[i].get());
	}
}

void Play::UpdateBoss()
{
	if(core.boss)
	{
		core.boss->RevealPlayerPosition( Vec2( core.player.GetX(), core.player.GetY() ) );
		ec.Update(core.boss.get());
	}
}

void Play::CheckCollisions()
{
	for (auto &enemy : core.enemy)
	{
		ec.CheckCollision(player, enemy.get());

		for (auto &ammo : core.ammo)
		{
			ec.CheckCollision(ammo.get(), enemy.get());
		}
		for (auto &asteroid : core.asteroid)
		{
			ec.CheckCollision(asteroid.get(), enemy.get());
		}

	}
	for (auto &asteroid : core.asteroid)
	{
		ec.CheckCollision(player, asteroid.get());
		for (auto &ammo : core.ammo)
		{
			ec.CheckCollision(ammo.get(), asteroid.get());
		}
		for (auto& astro : core.asteroid)
		{
			ec.CheckCollision(asteroid.get(), astro.get());
		}
	}

	ec.CheckCollision(core.boss.get(), player);
	for (auto &ammo : core.ammo)
	{
		ec.CheckCollision(core.boss.get(), ammo.get());
	}

	if (core.boss->GetHealth() <= 0.0f)
	{
		core.boss.release();
		core.currentLevel++;
		core.curState = StateCore::GState::LOADING;
	}

}

void Play::CleanVectors()
{
	auto &ammo_clean_start = std::remove_if(core.ammo.begin(), core.ammo.end(),
		[&](Projectile &Ammo)
	{
		return Ammo.HasTimeToLive();
	});
	auto &enemy_clean_start = std::remove_if(core.enemy.begin(), core.enemy.end(),
		[&](Enemy &En)
	{
		return En.GetHealth() <= 0.0f;
	});
}

void Play::ComposeFrame()
{
	Vec2 camOffset = cam->GetPosition();
	core.player.Draw(camOffset);
	core.player.GetShield().Draw(camOffset);
	for(unsigned int i = 0; i < core.ammo.size(); i++)
	{
		core.ammo[i]->Draw(camOffset);
	}
	for(unsigned int i = 0; i < core.enemy.size(); i++)
	{
		core.enemy[i]->Draw(camOffset);
	}
	for(unsigned int i = 0; i < core.asteroid.size(); i++)
	{
		core.asteroid[i]->Draw(camOffset);
	}
	if(core.boss)
	{
		core.boss->Draw(camOffset);
	}
	char buffer[20];
	sprintf(buffer, "%d", core.cScore.GetPlayerScore());
	gfx.DrawString(buffer, 649, 8, &f, gfx.FILLCOLOR_XRGB(255, 255, 255));
}
