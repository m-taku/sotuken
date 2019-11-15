#pragma once
#include"EnemyState.h"
class Enemy;
class EnemyStateDead : public EnemyState
{
public:
	EnemyStateDead(Enemy* enemypoint);
	~EnemyStateDead();
	void Update();
};

