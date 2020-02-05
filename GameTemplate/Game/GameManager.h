#pragma once
class QuestManager;
#include"StageManager.h"
class NPCManager;
class Player;
class GameManager : public IGameObject
{
public:
	GameManager();
	~GameManager();
	enum EGameNotify
	{
		NonQuestOrder,
		QuestOrder,
		QuestStart,
		InQuest,
		num
	};
	bool Start();
	void Update();
	void SetActive(bool Active)
	{

	}
	void ChangeNotify(EGameNotify GameNotify)
	{
		m_gameNotify = GameNotify;
		m_ChangeNotify = true;
	}
	Player* GetPlayer()
	{
		return m_player;
	}	
	void Change(bool furag);
private:
	StageManager m_Stage;
	Player* m_player = nullptr;
	NPCManager* m_NPCManager = nullptr;
	QuestManager*  m_QuestManager = nullptr;
	EGameNotify m_gameNotify = NonQuestOrder;
	bool m_ChangeNotify = false;
};

