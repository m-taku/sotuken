#pragma once
class Enemy;
class Player;
class monster : public Noncopyable
{
public:
	monster(const char* name);
	virtual ~monster()
	{

	}
	//�m���ɍ��Ȃ���΂Ȃ�Ȃ�����
	enum Defortoanim
	{
		idel,
		walk,
		Dead,
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
	//�����킹
	enum jiku1
	{
		strat,
		kaiten,
		wait
	};
	bool Alignment(CVector3 pos);
	void Setjiku1(jiku1 ks)
	{
		m_jikuawase = ks;
	}
protected:

	Enemy* m_enemy = nullptr;
	Player* m_player = nullptr;
	std::vector<AnimationClip> m_animClip;
	SkinModel m_skinmodel;		//�X�L�����f��
	//����
	//�����킹�p�i�g��Ȃ��ŁI�I�j
	float angle = 360.0f;
	int m_count = 0;
	float kaitennkaku = 00.0f;
	jiku1 m_jikuawase = strat;
};

