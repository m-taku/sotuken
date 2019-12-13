#pragma once
class Enemy;
class Player;
class QuestStage;
class QuestManager : public IGameObject
{
public:
	QuestManager();
	~QuestManager();

	bool Start();
	void Update();
	void Setdoun(int num)
	{
		m_doun = num;
	}
	void setTime(float num)
	{
		m_time = num;
	}
	void setBos(Enemy* bos)
	{
		m_target = bos;
	}
	void Playerdoun()
	{
		m_doun--;
		if (m_doun > 0)
		{
			//‰‰oƒNƒ‰ƒX
		}
	}

private:
	int m_doun = 0;
	float m_nowtime = 0.0f;
	float m_time = 0.0f;
	QuestStage* Stage = nullptr;
	Enemy* m_target = nullptr;
	Player* m_player = nullptr;
	int debugtime = 0;
};

