#include "stdafx.h"
#include "PlayerWait.h"
#include"Player.h"


PlayerWait::PlayerWait(Player* player) : PlayerState(player)
{
	m_player->SetMovespeed(CVector3::Zero());
}


PlayerWait::~PlayerWait()
{
}
void PlayerWait::Update()
{
}
void PlayerWait::DamageAction(float damage)
{

}
