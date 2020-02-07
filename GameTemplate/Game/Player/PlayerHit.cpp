#include "stdafx.h"
#include "PlayerHit.h"
#include"Player.h"

PlayerHit::PlayerHit(Player* player) :PlayerState(player)
{
	m_player->Playanim(Player::Hit);
	m_statenum = m_player->NowState();
}


PlayerHit::~PlayerHit()
{
	m_player->NowState() = m_statenum;
}
void PlayerHit::Update()
{
	if (!m_player->IsPlayinganim())
	{
		m_player->TransitionState(m_statenum);
	}
}
void PlayerHit::DamageAction(float damage)
{
	//DownHp(damage);
}