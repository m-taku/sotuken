#pragma once

#include "monster.h"
class dorakomesu : public monster
{
public:
	enum attackcombo
	{
		attack123,
		attack12,
		attack13,
		num
	};
	dorakomesu(CVector3 pos);
	virtual ~dorakomesu();
	void attackStart();
	bool attack();
	void HitAction(CVector3 originpos, CVector3 pos,float attack);
	void Init();
private:
	std::vector<std::vector<Bone*>> Bones;
	attackcombo m_attackcombo = attack123;
};

