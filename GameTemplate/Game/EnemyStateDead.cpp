#include "stdafx.h"
#include "EnemyStateDead.h"
#include"Enemy.h"
#include"Player.h"

EnemyStateDead::EnemyStateDead(Enemy* enemypoint):EnemyState(enemypoint)
{
	m_player = FindGO<Player>("player");
}


EnemyStateDead::~EnemyStateDead()
{
}
void EnemyStateDead::Update()
{
	m_enemy->SetMovespeed({ 0.0f,-10.0f,0.0f });
	switch (m_hagihagi)
	{
	case tikai:

		break;
	case tooi:

		break;
	default:
		break;
	}
}
