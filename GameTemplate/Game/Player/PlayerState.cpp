#include "stdafx.h"
#include "PlayerState.h"
#include"Player.h"

PlayerState::PlayerState(Player* player)
{
	m_player = player;
}


PlayerState::~PlayerState()
{

}
int PlayerState::DownHp(int Damage) {
	int EndDamage = max(Damage - m_player->GetPlayerParam().defense, 1);
	m_player->GetPlayerParam().hp -= EndDamage;
	return EndDamage;
}