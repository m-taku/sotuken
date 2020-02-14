#include "stdafx.h"
#include "EnemyStateChase.h"
#include"Enemy/Enemy.h"
#include"Player/Player.h"


EnemyStateChase::EnemyStateChase(Enemy* enemypoint, Player* player) : EnemyState(enemypoint, player)
{
	m_enemy->Playanim(monster::idel);
	m_path = m_enemy->CopyPath();
	Culling = std::thread([&] {rans(m_player->GetPosition()); });
	m_enemy->m_monster->Setjiku1(monster::strat);
}


EnemyStateChase::~EnemyStateChase()
{
	Stop();
}

void EnemyStateChase::Update()
{
	Navimake += GetFrameDeltaTime();
	if (Navimake >= 2.0f)
	{
		Navimake = 0.0f;
		Stop();
		Culling = std::thread([&] {rans(m_player->GetPosition()); });

	}
	if (ispath) {
		m_enemy->Playanim(monster::walk);
		auto speed = m_nextpos - m_enemy->GetPosition();
		speed.y = 0.0f;
		if (speed.Length() <= 100.0f)
		{
			//次のパスを検索する
			m_nextpos = m_path->PathPos();
			if (m_nextpos.x == m_oldposition.x&&m_nextpos.y == m_oldposition.y&&m_nextpos.z == m_oldposition.z)
			{
				Stop();
				Culling = std::thread([&] {rans(m_player->GetPosition()); });
				Navimake = 0.0f;
				m_enemy->Playanim(monster::idel);
				//もしも終点ならば探索しなおす
				//Culling.detach();
			}
			m_enemy->m_monster->Setjiku1(monster::strat);
			m_oldposition = m_nextpos;
			speed = m_nextpos - m_enemy->GetPosition();
		}
		speed.y = 0.0f;
		speed.Normalize();
		m_enemy->m_monster->Alignment(m_enemy->GetPosition() + speed);
		speed *= 900.0f;
		m_enemy->SetMovespeed(speed);

		//speed = m_nextpos - m_enemy->GetPosition();
		//speed.y = 0.0f;
		//speed.Normalize();
		//speed *= 500.0f;

	}
	else
	{
		m_enemy->Playanim(monster::idel);
	}
	if ((m_enemy->GetPosition() - m_player->GetPosition()).Length() <= 500.0f)
	{
		m_enemy->TransitionState(Enemy::StateAttack);
	}
	else if ((m_enemy->GetPosition() - m_player->GetPosition()).Length() >= 2000.0f)
	{

		m_enemy->TransitionState(Enemy::StateLoitering);
	}
}
void EnemyStateChase::Stop()
{
	m_path->stop();
	m_enemy->SetMovespeed(CVector3::Zero());
	Culling.join();
}
void EnemyStateChase::rans(CVector3 pos)
{
	ispath = false;
	while (true)
	{
		if (m_path->Course(m_enemy->GetPosition(), pos))
		{
			
			m_nextpos = m_path->PathPos();
			ispath = true;
			break;
		}
		else {
			break;
		}
	}
}