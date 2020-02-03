#include "stdafx.h"
#include "PlayerCombo.h"
#include"Player.h"
void PlayerCombo::changeweapon()
{
	NowWeapon += 1;
	NowWeapon %= 2;
	m_player->m_skinmodel.Setweapon(m_modelDxweapon[NowWeapon]);

}
void PlayerCombo::changeweapon(int No)
{	
	NowWeapon = No;
	m_player->m_skinmodel.Setweapon(m_modelDxweapon[No]);

}