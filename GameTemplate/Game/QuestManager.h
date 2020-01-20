#pragma once
class Enemy;
class Player;
class QuestStage;
#include"Test_Quest.h"
class QuestManager : public IGameObject
{
public:
	QuestManager();
	~QuestManager();
	bool Start();
	void Update();
	void Printkami()
	{
		m_isActive = true;
		m_No = 0;
		m_nowQuest = nullptr;
		m_Quest[m_No]->ChangePaper();
	}
	void CloseGuest()
	{
		m_isActive = false;
		m_Quest[m_No]->CreateQuest();
		m_nowQuest = m_Quest[m_No];
//		m_Quest[m_No]->ChangePaper();
	}
	void Playerdoun()
	{
		m_nowQuest->Playerdoun();
	}
private:

	int m_No = 0;
	std::vector<Test_Quest*> m_Quest;
	Test_Quest* m_nowQuest = nullptr;
	QuestStage* Stage = nullptr;
	//Enemy* m_target = nullptr;
	Player* m_player = nullptr;
	
};

