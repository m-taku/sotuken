#include "stdafx.h"
#include "Smallsword.h"
#include"Player.h"

Smallsword::Smallsword()
{

}


Smallsword::~Smallsword()
{

}
void Smallsword::SetAnimation(Player* player)
{
	m_animClip.resize(num);
	m_animClip[combo1].Load(L"Assets/animData/Smallsword_attack1.tka");
	m_animClip[combo1].SetLoopFlag(false);
	m_animClip[combo2].Load(L"Assets/animData/Smallsword_attack2.tka");
	m_animClip[combo2].SetLoopFlag(true);
	m_animClip[combo3].Load(L"Assets/animData/Smallsword_attack1.tka");
	m_animClip[combo3].SetLoopFlag(true);
	m_animClip[combo4].Load(L"Assets/animData/Smallsword_attackB4.tka");
	m_animClip[combo4].SetLoopFlag(true);
	m_animClip[combo5].Load(L"Assets/animData/Smallsword_attackX4.tka");
	m_animClip[combo5].SetLoopFlag(false);
	m_animClip[combo6].Load(L"Assets/animData/Smallsword_attack.tka");
	m_animClip[combo6].SetLoopFlag(true);
	m_animClip[combo7].Load(L"Assets/animData/Smallsword_attackX4.tka");
	m_animClip[combo7].SetLoopFlag(true);
	m_animClip[gerd].Load(L"Assets/animData/idel.tka");
	m_animClip[gerd].SetLoopFlag(true);
	m_player = player;
	m_player->SetAnim(&(*(m_animClip.begin())),num);
}
void Smallsword::changeY()
{
	switch (m_combo)
	{
	case 0:
		nowcombo = combo1;
		break;
	case 1:
		nowcombo = combo2;
		break;
	case 2:
		nowcombo = combo3;
		break;
	case 3:
		nowcombo = combo4;
		break;
	default:
		nowcombo = combo4;
		break;
	}
	m_combo++;
	m_player->Playanim(Player::num + nowcombo);
}
void Smallsword::changeB()
{
	switch (m_combo)
	{
	case 0:
		nowcombo = combo1;
		break;
	case 1:
		nowcombo = combo2;
		break;
	case 2:
		nowcombo = combo6;
		break;
	case 3:
		if (nowcombo == combo6) {
			nowcombo = combo7;
		}
		else {
			nowcombo = combo5;
		}
		break;
	default:
		nowcombo = combo7;
		break;
	}
	m_combo++;
	m_player->Playanim(Player::num + nowcombo);
}
void Smallsword::Reset()
{
	m_combo = 0;
	nowcombo = combo1;
}