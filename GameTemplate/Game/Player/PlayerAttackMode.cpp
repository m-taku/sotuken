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
		m_player->TransitionState(Player::StateAttack);
	}
	else if (g_pad[0].IsTrigger(enButtonA)) {
		m_player->TransitionState(Player::StateAttack);
	}
	else if (g_pad[0].IsTrigger(enButtonX)) {
		m_player->TransitionState(Player::StateQuestMove);
	}

}
