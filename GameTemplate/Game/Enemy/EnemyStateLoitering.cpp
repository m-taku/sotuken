#include "stdafx.h"
#include "EnemyStateLoitering.h"
#include"Enemy/Enemy.h"


EnemyStateLoitering::EnemyStateLoitering(Enemy* enemypoint):EnemyState(enemypoint)
{

}


EnemyStateLoitering::~EnemyStateLoitering()
{
}
void EnemyStateLoitering::Update()
{
	m_enemy->SetMovespeed({ 0.0f,-10.0f,0.0f });
}