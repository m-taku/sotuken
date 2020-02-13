#include "stdafx.h"
#include "PlayerAvoid.h"
#include"Player.h"


PlayerAvoid::PlayerAvoid(Player* player) :PlayerState(player)
{
	m_statenum = m_player->NowState();
	m_player->GetPlayerParam().stamina -= 0.00001;
	Movement.SetPlayer(player);
	m_player->Playanim(Player::avoid, true, 0.01f);

}


PlayerAvoid::~PlayerAvoid()
{
	m_player->NowState() = m_statenum;
}
void PlayerAvoid::Update()
{
	m_player->GetPlayerParam().stamina -= 10.0*GetFrameDeltaTime();
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
bool PlayerAvoid::DamageAction(float damage)
{
	if (!m_invincible) {
		return DownHp(damage);
	}
	return false;
}