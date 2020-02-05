#include "stdafx.h"
#include "PlayerTownMove.h"
#include"Player.h"


PlayerTownMove::PlayerTownMove(Player* player) :PlayerState(player)
{
	Movement.SetPlayer(player);
	if (m_player->Getcombo()!=nullptr) {
		m_player->Getcombo()->changeweapon(0);
	}
	m_player->Playanim(Player::walk);
}


PlayerTownMove::~PlayerTownMove()
{
}
void PlayerTownMove::Update()
{

	Movement.DefaultMove();
}
void PlayerTownMove::DamageAction(float damage)
{

}