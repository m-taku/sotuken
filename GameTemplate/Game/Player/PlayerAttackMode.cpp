#include "stdafx.h"
#include "PlayerAttackMode.h"
#include"Player.h"


PlayerAttackMode::PlayerAttackMode(Player* player) :PlayerState(player)
{

	m_player->Playanim(Player::walk);
}


PlayerAttackMode::~PlayerAttackMode()
{
}
void PlayerAttackMode::Update()
{
	if (g_pad[0].IsTrigger(enButtonY)) {
		m_player->Getcombo()->Reset();
		m_player->Getcombo()->changeY();
		m_player->TransitionState(Player::StateAttack);
	}
	else if (g_pad[0].IsTrigger(enButtonB)) {
		m_player->Getcombo()->Reset();
		m_player->Getcombo()->changeB();
		m_player->TransitionState(Player::StateAttack);
	}
	else if (g_pad[0].IsTrigger(enButtonX)) {
		m_player->TransitionState(Player::StateQuestMove);
	}

}
