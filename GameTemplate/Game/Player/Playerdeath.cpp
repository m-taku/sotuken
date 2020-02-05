#include "stdafx.h"
#include "Playerdeath.h"
#include"Player.h"
#include "QuestManager.h"



Playerdeath::Playerdeath(Player* player) :PlayerState(player)
{
	auto manager = FindGO<QuestManager>("QuestManager");
	manager->Playerdoun();
	m_player->Playanim(Player::death);
}



Playerdeath::~Playerdeath()
{
	m_player->GetPlayerParam().hp = m_player->GetPlayerData().hp;
	m_player->SetPosition(m_player->GetReturnPos());
}

void Playerdeath::Update()
{
}
void Playerdeath::DamageAction(float damage)
{

}