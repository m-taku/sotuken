#include "stdafx.h"
#include "dorakomesu.h"
#include"Player.h"
#include"Enemy/Enemy.h"
#include"../HitManeger.h"
dorakomesu::dorakomesu(CVector3 position) : monster(position)
{
}


dorakomesu::~dorakomesu()
{
}
void dorakomesu::attackStart()
{
	//ランダムでアタックを作る
	m_attackcombo = attack123;
	m_enemy->Playanim(Enemy::num + m_attackcombo);
}
bool dorakomesu::attack()
{
	switch (m_attackcombo)
	{
	case dorakomesu::attack123:
		//軸合わせしましょう（五つ筒筒か）
		HitAction();
		if (!m_enemy->IsPlayinganim())
		{
			return true;
		}
		break;
	case dorakomesu::attack12:
		HitAction();
		//軸合わせしましょう（五つ筒筒か）
		if (!m_enemy->IsPlayinganim())
		{
			return true;
		}
		break;

	default:
		break;
	}
	return false;
}
//当たり判定はこちらから
void dorakomesu::HitAction()
{
	if (m_enemy->IsAnimEvent(1))
	{
		//当たれーーー
		GetHitObjict().HitTest(m_enemy->GetPosition(), 1000.0f, 1, HitObject::player);
	}
}
void dorakomesu::Init()
{
	m_animClip.resize(num);
	m_animClip[attack123].Load(L"Assets/animData/dorakomesu_attackjac.tka");
	m_animClip[attack123].SetLoopFlag(false);
	m_animClip[attack12].Load(L"Assets/animData/dorakomesu_attackjac.tka");
	m_animClip[attack12].SetLoopFlag(false);
	m_enemy->SetAnim(&(*(m_animClip.begin())), num);
}