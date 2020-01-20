#pragma once
class QuestManager;
#include"StageManager.h"
class GameManager : public IGameObject
{
public:
	GameManager();
	~GameManager();
	bool Start();
	void Update();
	void cheng(bool furag);
 	StageManager m_Stage;
	QuestManager*  m_QuestManager = nullptr;
};

