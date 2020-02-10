#include "stdafx.h"
#include "EnemyStateLoitering.h"
#include"Enemy/Enemy.h"
#include"Player.h"

EnemyStateLoitering::EnemyStateLoitering(Enemy* enemypoint, Player* player) : EnemyState(enemypoint,player)
{
	m_enemy->Playanim(monster::idel);
}


EnemyStateLoitering::~EnemyStateLoitering()
{
}
void EnemyStateLoitering::Update()
{
	//�C�b�c�T���I�I�I�I�i�����ˁ[�[�[�[�j
	if ((m_enemy->GetPosition() - m_player->GetPosition()).Length() <= 1000.0f)
	{
		m_enemy->TransitionState(Enemy::StateAttack);
	}

}