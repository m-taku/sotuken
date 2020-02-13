#include "stdafx.h"
#include "EnemyStateLoitering.h"
#include"Enemy/Enemy.h"
#include"Player.h"

EnemyStateLoitering::EnemyStateLoitering(Enemy* enemypoint, Player* player) : EnemyState(enemypoint,player)
{
	m_enemy->Playanim(monster::idel);
	m_path = m_enemy->CopyPath();
	Culling = std::thread([&] {rans(m_enemy->GetReturnpos());});
	m_enemy->m_monster->Setjiku1(monster::strat);

	//Culling.detach();

}


EnemyStateLoitering::~EnemyStateLoitering()
{
	Stop();
}
void EnemyStateLoitering::Update()
{
	Navimake += GetFrameDeltaTime();
	if (ispath) {
		if ((m_enemy->GetReturnpos() - m_enemy->GetPosition()).Length() >= 200.0f) {
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
					Culling = std::thread([&] {rans(m_enemy->GetReturnpos()); });
					Navimake = 0.0f;
					m_enemy->Playanim(monster::idel);
					//もしも終点ならば探索しなおす
					//Culling.detach();
				}
				m_enemy->m_monster->Setjiku1(monster::strat);
				m_oldposition = m_nextpos;
				//speed = m_nextpos - m_enemy->GetPosition();
			}
			speed.y = 0.0f;
			speed.Normalize();
			if (speed.Length() > 0.0f) {
				if (m_enemy->m_monster->Alignment(m_enemy->GetPosition() + speed))
				{
					m_enemy->m_monster->Setjiku1(monster::wait);
					//m_enemy->Playanim(monster::idel);
				}
			}
			speed *= 900.0f;
			m_enemy->SetMovespeed(speed);
		}
		else
		{

			m_enemy->Playanim(monster::idel);
		}
		//speed = m_nextpos - m_enemy->GetPosition();
		//speed.y = 0.0f;
		//speed.Normalize();
		//speed *= 500.0f;

	}
	else
	{
		m_enemy->Playanim(monster::idel);
	}	
	if ((m_enemy->GetPosition() - m_player->GetPosition()).Length() <= 1500.0f)
	{
		m_enemy->TransitionState(Enemy::StateChase);
	}
}

void EnemyStateLoitering::Stop()
{
	m_path->stop();
	m_enemy->SetMovespeed(CVector3::Zero());
	Culling.join();
}
void EnemyStateLoitering::rans(CVector3 pos)
{
	ispath = false;
	while (true)
	{
		if (m_path->Course(m_enemy->GetPosition(), pos))
		{
			if (m_enemy->GetState() == Enemy::StateLoitering) {
				m_nextpos = m_path->PathPos();
			}
			ispath = true;
			break;

		}
		else {
			break;
		}
	}
}


