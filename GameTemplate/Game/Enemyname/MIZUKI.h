#pragma once
#include "monster.h"
#include"../Rig.h"
class MIZUKI : public monster
{
public:
	enum attackcombo
	{
		attack123,
		attack12,
		//attack13,
		num
	};
	MIZUKI(CVector3 pos);
	virtual ~MIZUKI();
	void attackStart();
	bool attack();
	void HitAction(CVector3 originpos, CVector3 pos, float attack);
	void Init();
private:
	std::vector<std::vector<Bone*>> Bones;
	Rig m_rig;
	attackcombo m_attackcombo = attack123;
};

