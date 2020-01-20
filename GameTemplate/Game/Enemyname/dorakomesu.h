#pragma once

#include "monster.h"
class dorakomesu : public monster
{
public:
	dorakomesu(CVector3 pos);
	virtual ~dorakomesu();
	void attack(); void Init();
};

