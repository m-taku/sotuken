#include "stdafx.h"
#include "Playerdeath.h"
#include"Player.h"
#include "QuestManager.h"



Playerdeath::Playerdeath(Player* player) :PlayerState(player)
{
	auto manager = FindGO<QuestManager>("QuestManager");
	manager->Playerdoun();
}



Playerdeath::~Playerdeath()
{
	m_player->SetRotation(CQuaternion::Identity());
}

void Playerdeath::Update()
{
	CVector3 pos = CVector3::Zero();
	CQuaternion pol;
	pol.SetRotationDeg(CVector3::AxisX(), 2.0f);
	rod.Multiply(pol);
	m_player->SetRotation(rod);
}
