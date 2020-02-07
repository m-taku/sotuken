#include "stdafx.h"
#include "EnemyStateAttack.h"
#include"Enemy.h"
#include"Player.h"


EnemyStateAttack::EnemyStateAttack(Enemy* enemypoint, Player* player) : EnemyState(enemypoint, player)
{
	m_enemy->m_monster->attackStart();
}


EnemyStateAttack::~EnemyStateAttack()
{
	
}

void EnemyStateAttack::Update()
{
	if (m_enemy->m_monster->attack())
	{
		m_enemy->TransitionState(Enemy::StateLoitering);
	}
}