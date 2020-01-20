#pragma once
class Stage_Base;
class StageManager : public Noncopyable
{
public:
	StageManager();
	~StageManager();
	void changTown();
	void changQuestStage();
private:
	Stage_Base* m_Stage = nullptr;
};

