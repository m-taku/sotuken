#include "stdafx.h"
#include "EnemyState.h"
#include"Enemy/Enemy.h"


EnemyState::EnemyState(Enemy* enemypoint)
{
	m_enemy = enemypoint;
}


EnemyState::~EnemyState()
{
}
