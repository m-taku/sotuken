#pragma once
#include"EnemyState.h"
class Enemy;
class Player;
class EnemyStateDead : public EnemyState
{
public:
	enum hagihagi
	{
		tikai,
		tooi
	};
	EnemyStateDead(Enemy* enemypoint, Player* player);
	~EnemyStateDead();
	void Update();
private:
	Player* m_player = nullptr; 
	hagihagi m_hagihagi = tooi;
};

