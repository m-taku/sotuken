#include "stdafx.h"
#include "PlayerAttack.h"
#include"Player.h"

PlayerAttack::PlayerAttack(Player* player) :PlayerState(player)
{

	m_player->Getcombo()->changeY();
}


PlayerAttack::~PlayerAttack()
{
}
void PlayerAttack::Update()
{
	hoge += GetFrameDeltaTime();
	if (g_pad[0].IsTrigger(enButtonY)) {
		m_player->Getcombo()->changeY();
		m_player->TransitionState(Player::StateAttack);
	}
	else if (g_pad[0].IsTrigger(enButtonB)) {
		m_player->Getcombo()->changeB();
		m_player->TransitionState(Player::StateAttack);
	}
	else if(hoge>=10.0f)
	{
		m_player->TransitionState(Player::StateAttackMode);
	}
}
