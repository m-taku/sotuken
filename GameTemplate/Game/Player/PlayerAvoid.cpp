#include "stdafx.h"
#include "PlayerAvoid.h"
#include"Player.h"


PlayerAvoid::PlayerAvoid(Player* player) :PlayerState(player)
{
	m_player->Playanim(Player::avoid,true);
	m_statenum = m_player->NowState();
	Movement.SetPlayer(player);
}


PlayerAvoid::~PlayerAvoid()
{
	m_player->NowState() = m_statenum;
}
void PlayerAvoid::Update()
{
	Movement.AttackMove();
	m_invincible = false;
	if (m_player->IsAnimEvent(1)) {
		m_invincible = true;
	}
	if (!m_player->IsPlayinganim())
	{
		m_player->TransitionState(m_statenum);
	}
}
void PlayerAvoid::DamageAction(float damage)
{
	if (!m_invincible) {
		DownHp(damage);
	}
}