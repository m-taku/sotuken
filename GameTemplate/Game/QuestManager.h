#pragma once
class QuestManager : public IGameObject
{
public:
	QuestManager();
	~QuestManager();
	bool Start();
	void Update();
};

