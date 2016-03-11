#pragma once

class StateCore;
class EntityController;

class Trigger
{
public:
	Trigger(StateCore &core);
	virtual void Do();
	bool IsAtTrigger();
	void Set(float x, float y);
	void Activate();
	void Clear();
	void UpdateTimer( float *eTimer);
	virtual ~Trigger();
protected:
	float x, y;
	bool passed, active;

	float SpawnRate, SpawnTimer;
	StateCore &core;
	EntityController *ec;
};

class ETrigger : public Trigger
{
public:
	ETrigger();
	ETrigger(StateCore &core);
	~ETrigger();
	void Do();
	void SetEnemyType( unsigned int i);

private:
	unsigned int enemyType;
};

class ATrigger : public Trigger
{
public:
	ATrigger();
	ATrigger(StateCore &core);
	~ATrigger();
	void Do();
};

class BTrigger : public Trigger
{
public:
	BTrigger();
	BTrigger(StateCore &core);
	~BTrigger();
	void Do();
};