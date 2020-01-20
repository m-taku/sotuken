#include "stdafx.h"
#include "QuestManager.h"
#include"Test_GuestManager.h"
#include "Stage/Town.h"
#include"Player.h"
#include"Stage/QuestStage.h"
#include"Enemy/Enemy.h"


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
	return true;
	//Stage = NewGO<QuestStage>(0, "kouya");
	//m_player = FindGO<Player>("player");
	//return true;
}
void QuestManager::Update()
{
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
	if (g_pad[0].IsTrigger(EnButton::enButtonRB3))
	{
		//NewGO<QuestStage>(0,"town");
		//FindGO<Player>("player")->SetPosition({-300.0f,500.0f,0.0f});
		CloseGuest();
		//pul->SetPosition({0.0f,1000.0f,0.0f});
		//DeleteGO(FindGO<Town>("town"));
		//DeleteGO(this);
	}

}