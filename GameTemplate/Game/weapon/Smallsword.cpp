#include "stdafx.h"
#include "Smallsword.h"
#include"Player.h"

Smallsword::Smallsword(Player* player) : PlayerCombo(player)
{
	m_modelDxweapon[0] = g_skinModelDataManager.Load(L"Assets/modelData/Smallsword2.cmo",player->GetSkeleton());
	m_modelDxweapon[1] = g_skinModelDataManager.Load(L"Assets/modelData/Smallsword1.cmo",player->GetSkeleton());
	changeweapon(0);
}


Smallsword::~Smallsword()
{

}
void Smallsword::SetAnimation(Player* player)
{
	m_animClip.resize(num); 
	m_animClip[pull1].Load(L"Assets/animData/Smallsword_pull1.tka"); 
	m_animClip[pull1].SetLoopFlag(false);
	m_animClip[pull2].Load(L"Assets/animData/Smallsword_pull2.tka");
	m_animClip[pull2].SetLoopFlag(false);
	m_animClip[push1].Load(L"Assets/animData/Smallsword_push1.tka");
	m_animClip[push1].SetLoopFlag(false);
	m_animClip[push2].Load(L"Assets/animData/Smallsword_push2.tka");
	m_animClip[push2].SetLoopFlag(false);
	m_animClip[combo1].Load(L"Assets/animData/Smallsword_attack1.tka");
	m_animClip[combo1].SetLoopFlag(false);
	m_animClip[combo2].Load(L"Assets/animData/Smallsword_attack2.tka");
	m_animClip[combo2].SetLoopFlag(false);
	m_animClip[combo3].Load(L"Assets/animData/Smallsword_attack1.tka");
	m_animClip[combo3].SetLoopFlag(false);
	m_animClip[combo4].Load(L"Assets/animData/Smallsword_attackB4.tka");
	m_animClip[combo4].SetLoopFlag(false);
	m_animClip[combo5].Load(L"Assets/animData/Smallsword_attackX4.tka");
	m_animClip[combo5].SetLoopFlag(false);
	m_animClip[combo6].Load(L"Assets/animData/Smallsword_attack.tka");
	m_animClip[combo6].SetLoopFlag(false);
	m_animClip[combo7].Load(L"Assets/animData/Smallsword_attackX4.tka");
	m_animClip[combo7].SetLoopFlag(false);
	m_animClip[gerd].Load(L"Assets/animData/idel.tka");
	m_animClip[gerd].SetLoopFlag(true);
	m_player->SetAnim(&(*(m_animClip.begin())),num);
}
void Smallsword::changeY()
{

	bool IsMove = false;
	switch (m_combo)
	{
	default:
		m_combo = 0;
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
		IsMove = true;
		//m_player->InMovemAnim();
		break;

	}
	m_combo++;
	m_player->Playanim(Player::num + nowcombo, IsMove);
}
void Smallsword::changeB()
{
	bool IsMove = false;
	switch (m_combo)
	{
	default:
		m_combo = 0;
	case 0:
		nowcombo = combo1;
		break;
	case 1:
		nowcombo = combo2;
		break;
	case 2:
		nowcombo = combo6;
		//m_player->InMovemAnim();
		IsMove = true;
		break;
	case 3:
		if (nowcombo == combo6) {
			nowcombo = combo7;
		}
		else {
			nowcombo = combo5;
		}
		IsMove = true;
		//m_player->InMovemAnim();
		break;
	
	}
	m_combo++;
	m_player->Playanim(Player::num + nowcombo,IsMove);
}
void Smallsword::pullweapon()
{
	m_player->Playanim(Player::num + pull1);
	if (!m_player->IsPlayinganim())
	{
		changeweapon();
		m_player->Playanim(Player::num + pull2);	
	}
}
void Smallsword::pushweapon()
{
	m_player->Playanim(Player::num + push1);
	if (!m_player->IsPlayinganim())
	{
		changeweapon();
		m_player->Playanim(Player::num + push2);
	}

}
void Smallsword::Reset()
{
	m_combo = 0;
	nowcombo = combo1;
}