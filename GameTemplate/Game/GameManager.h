#pragma once
class QuestManager;
#include"StageManager.h"
class Player;
class GameManager : public IGameObject
{
public:
	GameManager();
	~GameManager();
	bool Start();
	void Update();
	void cheng(bool furag);
	Player* m_player = nullptr;
 	StageManager m_Stage;
	QuestManager*  m_QuestManager = nullptr;
};

