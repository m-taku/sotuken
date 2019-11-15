#include "stdafx.h"
#include "EnemyStateDead.h"
#include"Enemy.h"

EnemyStateDead::EnemyStateDead(Enemy* enemypoint):EnemyState(enemypoint)
{
}


EnemyStateDead::~EnemyStateDead()
{
}
void EnemyStateDead::Update()
{
	m_enemy->SetMovespeed({ 0.0f,-10.0f,0.0f });
}
