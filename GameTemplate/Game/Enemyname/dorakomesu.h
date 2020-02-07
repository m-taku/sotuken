#pragma once

#include "monster.h"
class dorakomesu : public monster
{
public:
	enum attackcombo
	{
		attack123,
		attack12,
		num

	};
	dorakomesu(CVector3 pos);
	virtual ~dorakomesu();
	void attackStart();
	bool attack();
	void HitAction();
	void Init();
private:
	attackcombo m_attackcombo = attack123;
};

