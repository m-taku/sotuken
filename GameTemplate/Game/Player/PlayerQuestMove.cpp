#include "stdafx.h"
#include "PlayerQuestMove.h"
#include"Player.h"


PlayerQuestMove::PlayerQuestMove(Player* player) :PlayerState(player)
{
	Movement.SetPlayer(player);
	m_player->Playanim(Player::walk);
	m_player->Getcombo()->changeweapon(0);
}


PlayerQuestMove::~PlayerQuestMove()
{
	if (m_weapon != move) {
		m_player->NowState() = StateAttackMode;
		m_player->Getcombo()->changeweapon(1);
	}
}
void PlayerQuestMove::Update()
{
	switch (m_weapon)
	{
	case pull1:
		if (!m_player->IsPlayinganim())
		{
			m_player->Getcombo()->pullweapon();
			m_weapon = pull2;
		}
		break;
	case pull2:
		if (!m_player->IsPlayinganim())
		{
			m_player->TransitionState(StateAttackMode);
		}
		break;
	case move:
		Movement.QuestMove();
		if (g_pad[0].IsTrigger(enButtonY)) {
			m_weapon = pull1;
			m_player->Getcombo()->pullweapon();
			m_player->SetMovespeed(CVector3::Zero());
			//m_player->TransitionState(Player::StateAttackMode);
		}
		else if (g_pad[0].IsTrigger(enButtonA)) {
			m_player->TransitionState(StateAvoid);
		}
	default:
		break;
	}
	

}
bool PlayerQuestMove::DamageAction(float damage)
{
	return DownHp(damage);
}