#include "stdafx.h"
#include "PlayerQuestMove.h"
#include"Player.h"


PlayerQuestMove::PlayerQuestMove(Player* player) :PlayerState(player)
{
	Movement.SetPlayer(player);
	m_player->Playanim(Player::walk);
}


PlayerQuestMove::~PlayerQuestMove()
{
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
			m_player->TransitionState(Player::StateAttackMode);
		}
		break;
	case move:
		Movement.DefaultMove();
		if (g_pad[0].IsTrigger(enButtonY)) {
			m_weapon = pull1;
			m_player->Getcombo()->pullweapon();
			//m_player->TransitionState(Player::StateAttackMode);
		}
	default:
		break;
	}
	

}