#pragma once
class Stage_Base;
class NPCManager;
class StageManager : public Noncopyable
{
public:
	StageManager();
	~StageManager();
	void changTown();
	void changQuestStage();
private:
	Stage_Base* m_Stage = nullptr;
	NPCManager* m_NPCManager = nullptr;
};

