#include "stdafx.h"
#include "GameManager.h"
#include"Enemy/Enemy.h"
#include "Player.h"
#include"QuestManager.h"
#include"QuestManager.h"
GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

bool GameManager::Start()
{
	m_Stage.changTown();
	m_QuestManager = NewGO<QuestManager>(0, "QuestManager");
	NewGO<Player>(0, "player");
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
		DeleteGO(m_target);
		m_Stage.changTown();
	}
	else
	{
		m_QuestManager->CloseGuest();
		m_Stage.changQuestStage();
	}

}