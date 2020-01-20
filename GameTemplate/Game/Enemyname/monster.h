#pragma once
class Enemy;
class monster : public Noncopyable
{
public:
	monster(CVector3 pos);
	~monster();
	virtual void attack() {};
	virtual void Init() {};
protected:
	Enemy* m_enemy = nullptr;

};

