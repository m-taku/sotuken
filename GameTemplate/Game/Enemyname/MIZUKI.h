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
	MIZUKI(const char* name);
	virtual ~MIZUKI();
	void attackStart();
	bool attack();
	void HitAction(CVector3 originpos, CVector3 pos, float attack);
	void Init();
private:
	std::vector<std::vector<Bone*>> Bones;
	int No = 0;
	Rig m_rig;
	attackcombo m_attackcombo = attack123;
};

