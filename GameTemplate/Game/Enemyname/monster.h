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
	//確実に作らなければならないもの
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
	//軸合わせ
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
	SkinModel m_skinmodel;		//スキンモデル
	//試し
	//軸合わせ用（使わないで！！）
	float angle = 360.0f;
	int m_count = 0;
	float kaitennkaku = 00.0f;
	jiku1 m_jikuawase = strat;
};

