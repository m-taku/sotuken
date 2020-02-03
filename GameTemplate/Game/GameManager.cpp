#include "stdafx.h"
#include "GameManager.h"
#include"Enemy/Enemy.h"
#include "Player.h"
#include"QuestManager.h"
#include"QuestManager.h"
GameManager::GameManager()
{
	m_Stage.changTown();
	m_QuestManager = NewGO<QuestManager>(0, "QuestManager");
	m_player = NewGO<Player>(0, "player");
}


GameManager::~GameManager()
{
}

bool GameManager::Start()
{
	m_player->TransitionState(Player::StateTownMove);

	//cheng(true);
	return true;
}
void GameManager::Update()
{
	//auto pul = FindGO<Player>("player");
	//pul->TransitionState(Player::StateTownMove);
}
void  GameManager::cheng(bool furag)
{
	if (!furag)
	{
		auto m_target = FindGO<Enemy>("enemy");
		m_player->TransitionState(Player::StateTownMove);
		DeleteGO(m_target);
		m_Stage.changTown();
	}
	else
	{
		m_Stage.changQuestStage();
		m_QuestManager->CloseGuest();
		m_player->TransitionState(Player::StateQuestMove);
		m_player->Setweapon();
	}

}