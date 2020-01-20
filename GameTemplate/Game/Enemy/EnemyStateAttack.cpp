#include "stdafx.h"
#include "EnemyStateAttack.h"
#include"Enemy.h"
#include"Player.h"


EnemyStateAttack::EnemyStateAttack(Enemy* enemypoint) :EnemyState(enemypoint)
{
	
}


EnemyStateAttack::~EnemyStateAttack()
{
	
}

void EnemyStateAttack::Update()
{
	m_enemy->m_monster->attack();
}