#include "stdafx.h"
#include "MIZUKI.h"
#include"Player.h"
#include<random>
#include"Enemy/Enemy.h"
#include"../HitManeger.h"


MIZUKI::MIZUKI(CVector3 position) : monster(position)
{
}


MIZUKI::~MIZUKI()
{
}
void MIZUKI::attackStart()
{
	////ランダムでアタックを作る
	////乱数待って

	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand100(0, 1);        // [0, 99] 範囲の一様乱数
	int combo[3];
	combo[0] = rand100(mt);
	combo[1] = rand100(mt);
	combo[2] = rand100(mt);
	m_attackcombo = (attackcombo)combo[rand100(mt)];
	m_jikuawase = monster::strat; 
	No = 0;

	m_rig.StateRig(m_skinmodel.GetworldMatrix());
	m_enemy->Playanim(monster::walk);
}
bool MIZUKI::attack()
{
	CVector3 tailpos1, tailpos2;
	switch (m_attackcombo)
	{
	case MIZUKI::attack123:
		//軸合わせしましょう（五つ筒筒か）

		if (Alignment(m_player->GetPosition())) {
			if (m_enemy->IsAnimEvent(1))
			{
				m_jikuawase = wait;
			}
			m_enemy->Playanim(m_attackcombo + monster::num);
			//CVector3 tailpos1;
			tailpos1.Set(Bones[attack123][0]->GetWorldMatrix().v[3]);
			HitAction(tailpos1, tailpos1, 2000.0f);
			if (!m_enemy->IsPlayinganim())
			{
				return true;
			}
		}
		break;
	case MIZUKI::attack12:
		if (Alignment(m_player->GetPosition())) {
			if (No == 0)
			{
				m_rig.StateRig(m_skinmodel.GetworldMatrix());
				m_jikuawase = wait;
				No++;
			}
			m_enemy->Playanim(m_attackcombo + monster::num,true);
			CVector3 move = CVector3::Zero();
			move = m_rig.Updete();
			move /= GetFrameDeltaTime();
			move.y = 0.0f;
			m_enemy->SetMovespeed(move);
			if (m_enemy->IsAnimEvent(2)) {
				tailpos1.Set(Bones[attack12][1]->GetWorldMatrix().v[3]);
			}
			else
			{
				tailpos1.Set(Bones[attack12][0]->GetWorldMatrix().v[3]);
			}
			HitAction(tailpos1, tailpos1, 2000.0f);
			//軸合わせしましょう（五つ筒筒か）
			if (!m_enemy->IsPlayinganim())
			{

				m_enemy->Playanim(m_attackcombo + monster::num, false);
				return true;
			}
		}

		break;
	//case dorakomesu::attack13:
	//	//軸合わせしましょう（五つ筒筒か）
	//	if (Alignment()) {
	//		m_enemy->Playanim(m_attackcombo + monster::num);
	//		//CVector3 tailpos1;
	//		tailpos1.Set(Bones[attack13][0]->GetWorldMatrix().v[3]);
	//		HitAction(tailpos1, tailpos1, 50.0f);
	//		if (!m_enemy->IsPlayinganim())
	//		{
	//			angle = 360.0f;
	//			return true;
	//		}
	//	}
	//	break;
	default:
		break;
	}
	return false;
}
//当たり判定はこちらから
void MIZUKI::HitAction(CVector3 originpos, CVector3 pos, float attack)
{
	if (m_enemy->IsAnimEvent(1))
	{
		//当たれーーー
		GetHitObjict().HitTest(originpos, pos, 150.0f, attack, HitObject::player);
	}
}
void MIZUKI::Init()
{
	m_skinmodel.Init(L"Assets/modelData/MIZUKI.cmo");
	m_enemy->SetAnimdate(m_skinmodel);
	//m_skinmodel.EnableShadowCaster(true);
	m_animClip.resize(0);
	m_animClip.resize(monster::num+ num);
	Bones.resize(num);
	m_animClip[monster::idel].Load(L"Assets/animData/MIZUKI_idel.tka");
	m_animClip[monster::idel].SetLoopFlag(true);
	m_animClip[monster::walk].Load(L"Assets/animData/MIZUKI_wark.tka");
	m_animClip[monster::walk].SetLoopFlag(true);
	m_animClip[monster::Dead].Load(L"Assets/animData/MIZUKI_dead.tka");
	m_animClip[monster::Dead].SetLoopFlag(false);

	m_animClip[attack123 + monster::num].Load(L"Assets/animData/MIZUKI_attackneil.tka");
	m_animClip[attack123 + monster::num].SetLoopFlag(false);
	Bones[attack123].push_back(m_skinmodel.FindBone(L"MOUNTAIN_DRAGON_ R Hand"));

	m_animClip[attack12 + monster::num].Load(L"Assets/animData/MIZUKI_attackneil2.tka");
	m_animClip[attack12 + monster::num].SetLoopFlag(false);

	Bones[attack12].push_back(m_skinmodel.FindBone(L"MOUNTAIN_DRAGON_ R Hand"));
	Bones[attack12].push_back(m_skinmodel.FindBone(L"MOUNTAIN_DRAGON_ L Hand"));
	

	//for (int i = 1; i <= 10; i++)
	//{
	//	wchar_t moveFilePath[256];
	//	swprintf_s(moveFilePath, L"Tail%02d", i);
	//	Bones[attack12].push_back(m_skinmodel.FindBone(moveFilePath));
	//}
	//m_animClip[attack13 + monster::num].Load(L"Assets/animData/dorakomesu_attackmeil.tka");
	//m_animClip[attack13 + monster::num].SetLoopFlag(false);
	m_enemy->SetAnim(&(*(m_animClip.begin())),monster::num + num);
	m_rig.SetBoon(m_skinmodel, L"MOUNTAIN_DRAGON_");
}