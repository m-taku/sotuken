#include "stdafx.h"
#include "dorakomesu.h"
#include"Player.h"
#include"Enemy/Enemy.h"
#include<random>
#include"../HitManeger.h"
dorakomesu::dorakomesu(CVector3 position) : monster(position)
{

}


dorakomesu::~dorakomesu()
{
	m_animClip.resize(0);
	for (int i = 0; i < Bones.size(); i++)
	{
		Bones[i].clear();
	}
	Bones.clear();
}
void dorakomesu::attackStart()
{
	//ランダムでアタックを作る
	//乱数待って

	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> rand100(0, 2);        // [0, 99] 範囲の一様乱数
	int combo[3];
	combo[0] = rand100(mt);
	combo[1] = rand100(mt);
	combo[2] = rand100(mt);
	m_attackcombo = (attackcombo)combo[rand100(mt)];
	m_jikuawase = monster::strat;
	m_enemy->Playanim(monster::walk);
}
bool dorakomesu::attack()
{
	CVector3 tailpos1, tailpos2;
	switch (m_attackcombo)
	{
	case dorakomesu::attack123:
		//軸合わせしましょう（五つ筒筒か）
		if (Alignment(m_player->GetPosition())) {
			m_enemy->Playanim(m_attackcombo + monster::num);
			//CVector3 tailpos1;
			tailpos1.Set(Bones[attack123][0]->GetWorldMatrix().v[3]);
			HitAction(tailpos1, tailpos1, 20.0f);
			if (!m_enemy->IsPlayinganim())
			{
				return true;
			}
		}
		break;
	case dorakomesu::attack12:
		m_enemy->Playanim(m_attackcombo + monster::num);
		for (int i = 0; i < Bones[attack12].size() - 1; i++) {

			tailpos1.Set(Bones[attack12][i]->GetWorldMatrix().v[3]);
			tailpos2.Set(Bones[attack12][i + 1]->GetWorldMatrix().v[3]);
			auto houkou = tailpos2 - tailpos1;
			CVector3 taillen = houkou;
			houkou.Normalize();
			auto toPlayer = m_player->GetPosition() - tailpos1;
			float len = houkou.Dot(toPlayer);

			if (len <= taillen.Length() && len > 0.0f) {
				CVector3 pos = tailpos1 + houkou * len;
				float hitlen = CVector3(m_player->GetPosition() - pos).Length();
				HitAction(tailpos1, tailpos1 + taillen, 5.0f);
				break;
			}
		}
			//軸合わせしましょう（五つ筒筒か）
		if (!m_enemy->IsPlayinganim())
		{
			return true;
		}
		break;
	case dorakomesu::attack13:
		//軸合わせしましょう（五つ筒筒か）
		if (Alignment(m_player->GetPosition())) {
			m_enemy->Playanim(m_attackcombo + monster::num);
			//CVector3 tailpos1;
			tailpos1.Set(Bones[attack13][0]->GetWorldMatrix().v[3]);
			HitAction(tailpos1, tailpos1, 50.0f);
			if (!m_enemy->IsPlayinganim())
			{
				angle = 360.0f;
				return true;
			}
		}
		break;
	default:
		break;
	}
	return false;
}
//当たり判定はこちらから
void dorakomesu::HitAction(CVector3 originpos, CVector3 pos, float attack)
{
	if (m_enemy->IsAnimEvent(1))
	{
		//当たれーーー
		GetHitObjict().HitTest(originpos, pos, 80.0f, attack, HitObject::player);
	}
}
void dorakomesu::Init()
{
	m_skinmodel.Init(L"Assets/modelData/Dragon_3.cmo");
	m_enemy->SetAnimdate(m_skinmodel);
	m_skinmodel.EnableShadowCaster(true);
	m_animClip.resize(0);
	m_animClip.resize(num + monster::num);
	Bones.resize(num);
	m_animClip[monster::idel].Load(L"Assets/animData/dorakomesu_idel.tka");
	m_animClip[monster::idel].SetLoopFlag(true);
	m_animClip[monster::walk].Load(L"Assets/animData/dorakomesu_wark.tka");
	m_animClip[monster::walk].SetLoopFlag(true);
	m_animClip[monster::Dead].Load(L"Assets/animData/dorakomesu_dead.tka");
	m_animClip[monster::Dead].SetLoopFlag(false);

	m_animClip[attack123 + monster::num].Load(L"Assets/animData/dorakomesu_attackjac.tka");
	m_animClip[attack123 + monster::num].SetLoopFlag(false);
	Bones[attack123].push_back(m_skinmodel.FindBone(L"Head"));

	m_animClip[attack12 + monster::num].Load(L"Assets/animData/dorakomesu_attacksip.tka");
	m_animClip[attack12 + monster::num].SetLoopFlag(false);


	for (int i = 1; i <= 10; i++)
	{
		wchar_t moveFilePath[256];
		swprintf_s(moveFilePath, L"Tail%02d", i);
		Bones[attack12].push_back(m_skinmodel.FindBone(moveFilePath));
	}
	m_animClip[attack13 + monster::num].Load(L"Assets/animData/dorakomesu_attackmeil.tka");
	m_animClip[attack13 + monster::num].SetLoopFlag(false);
	Bones[attack13].push_back(m_skinmodel.FindBone(L"L Hand"));
	m_enemy->SetAnim(&(*(m_animClip.begin())), num + monster::num);
}