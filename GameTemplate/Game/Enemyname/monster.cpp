#include "stdafx.h"
#include "monster.h"
#include"Enemy/Enemy.h"


monster::monster(CVector3 pos)
{
	m_enemy = NewGO<Enemy>(0,"enemy");
	m_enemy->SetPosition(pos);
	m_enemy->m_monster = this;
}


monster::~monster()
{
}
