#include "stdafx.h"
#include "PlayerQuestMove.h"
#include"Player.h"


PlayerQuestMove::PlayerQuestMove(Player* player) :PlayerState(player)
{
	Movement.SetPlayer(player);
}


PlayerQuestMove::~PlayerQuestMove()
{
}
void PlayerQuestMove::Update()
{
	Movement.DefaultMove();
	if (g_pad[0].IsTrigger(enButtonX)) {
		m_player->TransitionState(Player::StateAttackMode);
	}
}