#include "stdafx.h"
#include "monster.h"
#include"Enemy/Enemy.h"
#include"Player.h"


monster::monster(CVector3 pos)
{
	m_enemy = NewGO<Enemy>(0,"enemy");
	m_enemy->SetPosition(pos);
	m_enemy->m_monster = this;
	m_player = FindGO<Player>("player");
}


monster::~monster()
{
}
