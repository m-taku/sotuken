#pragma once
#include"EnemyState.h"
class Enemy;
class Player;
class EnemyStateAttack : public EnemyState
{
public:
	EnemyStateAttack(Enemy* enemypoint, Player* player);
	~EnemyStateAttack();
	void Update();
};

