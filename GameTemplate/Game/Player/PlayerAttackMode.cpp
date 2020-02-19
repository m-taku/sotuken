#include "stdafx.h"
#include "PlayerAttackMode.h"
#include"Player.h"


PlayerAttackMode::PlayerAttackMode(Player* player) :PlayerState(player)
{

	Movement.SetPlayer(player);
	m_player->Playanim(Player::walk);
	m_player->Getcombo()->changeweapon(1);
}


PlayerAttackMode::~PlayerAttackMode()
{
	if (m_weapon != move) {
		m_player->NowState() = StateQuestMove;
		m_player->Getcombo()->changeweapon(0);
	}
}
void PlayerAttackMode::Update()
{
	switch (m_weapon)
	{
	case push1:
		if (!m_player->IsPlayinganim())
		{
			m_player->Getcombo()->pushweapon();
			m_weapon = push2;
		}
		break;
	case push2:
		if (!m_player->IsPlayinganim())
		{
			m_player->TransitionState(StateQuestMove);
		}
		break;
	case move:
		Movement.QuestWeaponMove();
		if (g_pad[0].IsTrigger(enButtonY)) {
			m_player->Getcombo()->Reset();
			m_player->Getcombo()->changeY();
			m_player->TransitionState(StateAttack);
		}
		else if (g_pad[0].IsTrigger(enButtonB)) {
			m_player->Getcombo()->Reset();
			m_player->Getcombo()->changeB();
			m_player->TransitionState(StateAttack);
		}
		else if (g_pad[0].IsTrigger(enButtonX)) {
			m_weapon = push1;
			m_player->Getcombo()->pushweapon();
		}	
		else if (g_pad[0].IsTrigger(enButtonA)) {
			m_player->TransitionState(StateAvoid);
		}
	default:
		break;
	}
}
bool PlayerAttackMode::DamageAction(float damage)
{
	return DownHp(damage);
}
