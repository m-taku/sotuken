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
	virtual void attackStart() {};
	virtual bool attack() {
		return true;
	};
	virtual void HitAction()
	{

	}
	virtual void Init() {};
protected:
	Enemy* m_enemy = nullptr;
	std::vector<AnimationClip> m_animClip;

};

