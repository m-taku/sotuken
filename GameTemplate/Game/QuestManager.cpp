#include "stdafx.h"
#include "QuestManager.h"
#include "Stage/Town.h"
#include"Player.h"
#include"GameManager.h"
#include"Stage/QuestStage.h"
#include"Enemy/Enemy.h"
#include "GameManager.h"


QuestManager::QuestManager()
{
}


QuestManager::~QuestManager()
{
	for (int i = 0; i < m_Quest.size(); i++) {
		DeleteGO(m_Quest[i]);
	}
}

bool QuestManager::Start()
{
	auto Quest = NewGO<Test_Quest>(0);
	Quest->Init(L"Assets/TestQeste/save0.smqd");
	m_Quest.push_back(Quest);
	Quest->Start();
	Quest->ChangePaper();
	Quest = NewGO<Test_Quest>(0);
	Quest->Init(L"Assets/TestQeste/save1.smqd");
	m_Quest.push_back(Quest);
	Quest->Start();
	Quest->ChangePaper();
	Quest = NewGO<Test_Quest>(0);
	Quest->Init(L"Assets/TestQeste/save2.smqd");
	Quest->Start();
	Quest->ChangePaper();
	m_Quest.push_back(Quest);
	m_isActive = false;
	m_GameManager = FindGO<GameManager>("GameManager");
	return true;
	//Stage = NewGO<QuestStage>(0, "kouya");
	//m_player = FindGO<Player>("player");
	//return true;
}
void QuestManager::Update()
{
	switch (m_mode)
	{
	case Decision:
		if (g_pad[0].IsTrigger(enButtonUp))
		{
			if (m_Quest.size() != min(m_No + 1, m_Quest.size())) {
				m_Quest[m_No++]->ChangePaper();
				m_Quest[m_No]->ChangePaper();
			}
		}
		if (g_pad[0].IsTrigger(enButtonDown))
		{
			if (-1 != max(m_No - 1, -1)) {
				m_Quest[m_No--]->ChangePaper();
				m_Quest[m_No]->ChangePaper();
			}
		}
		if (g_pad[0].IsTrigger(enButtonA))
		{
			m_Quest[m_No]->ChangePaper();
			m_GameManager->ChangeNotify(GameManager::QuestOrder);
		}
		if (g_pad[0].IsTrigger(enButtonB))
		{
			m_isActive = false;
			m_Quest[m_No]->ChangePaper();
			m_GameManager->ChangeNotify(GameManager::NonQuestOrder);
		}
		break;
	case holdQuest:
		if (g_pad[0].IsTrigger(enButtonSelect))
		{
			m_Quest[m_No]->ChangePaper();
			m_GameManager->GetPlayer()->TransitionState(StateWate);
			m_mode = WaitQuest;
		}
		break;
	case WaitQuest:
		if (g_pad[0].IsTrigger(enButtonSelect)|| g_pad[0].IsTrigger(enButtonB))
		{
			m_Quest[m_No]->ChangePaper();
			m_GameManager->GetPlayer()->TransitionState(StateTownMove);
			m_mode = holdQuest;
		}
		else
		{
			if (g_pad[0].IsTrigger(enButtonY))
			{
				m_GameManager->ChangeNotify(GameManager::QuestStart);
			}
		}
		break;
	case Cancel:
		if (g_pad[0].IsTrigger(enButtonA))
		{
			m_isActive = false;
			m_Quest[m_No]->ChangePaper();
			m_GameManager->ChangeNotify(GameManager::QuestOrder);
		}
		else if (g_pad[0].IsTrigger(enButtonB))
		{
			m_isActive = false;
			m_Quest[m_No]->ChangePaper();
			m_No = 0;
			m_GameManager->ChangeNotify(GameManager::NonQuestOrder);
		}
		break;
	default:
		break;
	}


}