#pragma once
class Enemy;
class monster : public Noncopyable
{
public:
	monster(CVector3 pos);
	~monster();
	Enemy* GetEnemy()
	{
		return m_enemy;
	}
	virtual void attack() {};
	virtual void Init() {};
protected:
	Enemy* m_enemy = nullptr;

};

