#pragma once
class QuestManager;
#include"StageManager.h"
class NPCManager;
class Player;
class UI;
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
		EndQuest,
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
	CSoundSource m_bgm;
	StageManager m_Stage;
	UI* m_ui = nullptr;
	Player* m_player = nullptr;
	NPCManager* m_NPCManager = nullptr;
	QuestManager*  m_QuestManager = nullptr;
	EGameNotify m_gameNotify = NonQuestOrder;
	bool m_ChangeNotify = false;
};

