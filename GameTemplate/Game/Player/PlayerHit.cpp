#include "stdafx.h"
#include "PlayerHit.h"
#include"Player.h"

PlayerHit::PlayerHit(Player* player) :PlayerState(player)
{
	m_player->Playanim(Player::Hit, true);
	m_statenum = m_player->NowState();
	Movement.SetPlayer(player);
}


PlayerHit::~PlayerHit()
{
	m_player->NowState() = m_statenum;
}
void PlayerHit::Update()
{
	Movement.AttackMove();
	if (!m_player->IsPlayinganim())
	{
		m_player->TransitionState(m_statenum);
	}
}
bool PlayerHit::DamageAction(float damage)
{
	return false;
}