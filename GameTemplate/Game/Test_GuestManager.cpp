#include "stdafx.h"
#include "Test_GuestManager.h"
#include"Test_Quest.h"


Test_GuestManager::Test_GuestManager()
{
}


Test_GuestManager::~Test_GuestManager()
{
	for (int i = 0; i < m_Quest.size(); i++) {
		DeleteGO(m_Quest[i]);
	}
}
bool Test_GuestManager::Start()
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
	//Quest = NewGO<Test_Quest>(0);
	//Quest->Init(L"Assets/TestQeste/save3.smqd");
	//Quest->SetActive(false);
	//m_Quest.push_back(Quest);
	//Quest = NewGO<Test_Quest>(0);
	//Quest->Init(L"Assets/TestQeste/save4.smqd");
	//Quest->SetActive(false);
	//m_Quest.push_back(Quest);

}
void Test_GuestManager::Update()
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
}