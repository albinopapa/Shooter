#pragma once

#include "StateCore.h"
class GameState;

class StateController
{
public:
	StateController( StateCore &core);
	~StateController();
	void Transition(std::unique_ptr<GameState> &pNewState);
	void Do();
	void DoEscape();
	void DoF10();
	void SetLevel( std::unique_ptr<Level> &NextLevel );
	void SetLevelIndex( unsigned int i);
	unsigned int GetLevelIndex()const;
	Level *GetLevel() const;
	void IsInShop( bool isShopping );
	
private:
	std::unique_ptr<GameState> state, pauseState;
	bool isPaused, isShopping;
	StateCore &core;
};
