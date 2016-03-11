#pragma once

#include "StateCore.h"
class GameState;

class StateController
{
public:
	StateController( StateCore &core);
	~StateController();
	void Transition( GameState *pNewState);
	void Do();
	void DoEscape();
	void DoF10();
	void SetLevel( Level *l );
	void SetLevelIndex( unsigned int i);
	unsigned int GetLevelIndex()const;
	Level *GetLevel() const;
	void IsInShop( bool isShopping );
	
private:
	GameState *state, *pauseState;
	bool isPaused, isShopping;
	StateCore &core;
};
