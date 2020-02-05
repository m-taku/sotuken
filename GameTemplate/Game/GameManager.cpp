#include "stdafx.h"
#include "GameManager.h"
#include"Enemy/Enemy.h"
#include "Player.h"
#include"QuestManager.h"
#include"NPCManager.h"
#include"Quest_NPC.h"
GameManager::GameManager()
{

}


GameManager::~GameManager()
{
}

bool GameManager::Start()
{
	//m_NPCManager = NewGO<NPCManager>(0, "NPCManager");
	m_Stage.changTown();
	
	m_QuestManager = NewGO<QuestManager>(0, "QuestManager");
	m_player = NewGO<Player>(0, "player");
	//m_player->TransitionState(Player::StateTownMove);

	//cheng(true);
	return true;
}
void GameManager::Update()
{

	switch (m_gameNotify)
	{
	case NonQuestOrder:
		if (m_ChangeNotify)
		{
			m_QuestManager->ChangeOrderMode(QuestManager::Decision);
			m_player->TransitionState(Player::StateTownMove);
			FindGO<Quest_NPC>("受付ジョー")->SetTalkState(Quest_NPC::QuestSelect);
			break;
		}
	case QuestOrder:
		if (m_ChangeNotify)
		{
			m_QuestManager->ChangeOrderMode(QuestManager::holdQuest);
			m_player->TransitionState(Player::StateTownMove);
			FindGO<Quest_NPC>("受付ジョー")->SetTalkState(Quest_NPC::QuestCancel);
		}
		break;
	case QuestStart:
		if (m_ChangeNotify)
		{
			m_QuestManager->CloseGuest();
			m_Stage.changQuestStage();
			m_player->TransitionState(Player::StateQuestMove);
			m_player->Setweapon();
			m_gameNotify = InQuest;
		}
		break;
	case InQuest:

		break;
	default:
		break;
	}
	m_ChangeNotify = false;

	//auto pul = FindGO<Player>("player");
	//pul->TransitionState(Player::StateTownMove);
}
void  GameManager::Change(bool furag)
{
	if (!furag)
	{
		auto m_target = FindGO<Enemy>("enemy");
		DeleteGO(m_target);

		m_QuestManager->ChangeOrderMode(QuestManager::Decision);
		m_player->TransitionState(Player::StateTownMove);
		m_Stage.changTown();
	}
	else
	{
		//m_QuestManager->CloseGuest();
		//m_Stage.changQuestStage();
		//m_player->TransitionState(Player::StateQuestMove);
		//m_player->Setweapon();
	}

}