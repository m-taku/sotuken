#pragma once
class Enemy;
class Player;
class monster : public Noncopyable
{
public:
	monster(CVector3 pos);
	~monster();
	//�m���ɍ��Ȃ���΂Ȃ�Ȃ�����
	enum Defortoanim
	{
		idel,
		walk,
		num
	};
	Enemy* GetEnemy()
	{
		return m_enemy;
	}
	SkinModel* GetSkinModel()
	{
		return &m_skinmodel;
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
	Player* m_player = nullptr;
	std::vector<AnimationClip> m_animClip;
	SkinModel m_skinmodel;		//�X�L�����f��
};

