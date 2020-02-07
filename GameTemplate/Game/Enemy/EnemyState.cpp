#include "stdafx.h"
#include "EnemyState.h"
#include"Enemy/Enemy.h"


EnemyState::EnemyState(Enemy* enemypoint,Player* player)
{
	m_enemy = enemypoint;
	m_player = player;
}


EnemyState::~EnemyState()
{
}
