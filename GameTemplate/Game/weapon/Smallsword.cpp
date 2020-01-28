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
	m_animClip[combo1].Load(L"Assets/animData/idel.tka");
	m_animClip[combo1].SetLoopFlag(false);
	m_animClip[combo2].Load(L"Assets/animData/idel.tka");
	m_animClip[combo2].SetLoopFlag(true);
	m_animClip[combo3].Load(L"Assets/animData/idel.tka");
	m_animClip[combo3].SetLoopFlag(true);
	m_animClip[combo4].Load(L"Assets/animData/idel.tka");
	m_animClip[combo4].SetLoopFlag(true);
	m_animClip[combo5].Load(L"Assets/animData/idel.tka");
	m_animClip[combo5].SetLoopFlag(false);
	m_animClip[combo6].Load(L"Assets/animData/idel.tka");
	m_animClip[combo6].SetLoopFlag(true);
	m_animClip[combo7].Load(L"Assets/animData/run.tka");
	m_animClip[combo7].SetLoopFlag(true);
	m_animClip[gerd].Load(L"Assets/animData/idel.tka");
	m_animClip[gerd].SetLoopFlag(true);
	m_player = player;
	m_player->SetAnim(&(*(m_animClip.begin())),num);
}
void Smallsword::changeY()
{
	m_player->Playanim(Player::num + combo4);
}
void Smallsword::changeB()
{

	m_player->Playanim(Player::num + combo7);
}