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
	Movement.TounMove();
	//m_player->Playanim(Player::run, false, 1.0f - (Length.Length() / 500.0f));
	//m_player->Playanim(Player::walk, false, (Length.Length() / 500.0f));
}
bool PlayerTownMove::DamageAction(float damage)
{
	return false;
}