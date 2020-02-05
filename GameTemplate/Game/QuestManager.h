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
	enum OrderMode
	{
		Decision,
		holdQuest,
		WaitQuest,
		Cancel

	};
	bool Start();
	void Update();
	void Printkami()
	{
		m_isActive = true;
		m_nowQuest = nullptr;
		m_Quest[m_No]->ChangePaper();
	}
	void CloseGuest()
	{
		m_isActive = false;
		m_Quest[m_No]->CreateQuest();
		m_nowQuest = m_Quest[m_No];
		m_No = 0;
	}
	void Playerdoun()
	{
		m_nowQuest->Playerdoun();
	}
	void ChangeOrderMode(OrderMode Mode)
	{
		m_mode = Mode;
	}
	Test_Quest* GetQuestDate()
	{
		return m_nowQuest;
	}
private:
	int m_No = 0;
	std::vector<Test_Quest*> m_Quest;
	Test_Quest* m_nowQuest = nullptr;
	GameManager* m_GameManager = nullptr;
	OrderMode m_mode = Decision;
	//Enemy* m_target = nullptr;
	
};

