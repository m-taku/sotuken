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
	//�����_���ŃA�^�b�N�����
	m_attackcombo = attack12;
	m_enemy->Playanim(m_attackcombo + monster::num);
}
bool dorakomesu::attack()
{
	CVector3 tailpos1, tailpos2;
	switch (m_attackcombo)
	{
	case dorakomesu::attack123:
		//�����킹���܂��傤�i�܂������j
		//CVector3 tailpos1;
		tailpos1.Set(Bones[attack123][0]->GetWorldMatrix().v[3]);
		HitAction(tailpos1, tailpos1);
		if (!m_enemy->IsPlayinganim())
		{
			return true;
		}
		break;
	case dorakomesu::attack12:
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
				HitAction(tailpos1, tailpos1 + taillen);
				break;
			}
		}
		//�����킹���܂��傤�i�܂������j
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
//�����蔻��͂����炩��
void dorakomesu::HitAction(CVector3 originpos, CVector3 pos)
{
	if (m_enemy->IsAnimEvent(1))
	{
		//������[�[�[
		GetHitObjict().HitTest(originpos, pos, 80.0f, 1, HitObject::player);
	}
}
void dorakomesu::Init()
{
	m_skinmodel.Init(L"Assets/modelData/Dragon_3.cmo");
	m_enemy->SetAnimdate(m_skinmodel);
	m_skinmodel.EnableShadowCaster(true);
	m_animClip.resize(num + monster::num);
	Bones.resize(num);
	m_animClip[monster::idel].Load(L"Assets/animData/dorakomesu_idel.tka");
	m_animClip[monster::idel].SetLoopFlag(true);
	m_animClip[monster::walk].Load(L"Assets/animData/dorakomesu_wark.tka");
	m_animClip[monster::walk].SetLoopFlag(true);

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

	m_enemy->SetAnim(&(*(m_animClip.begin())), num + monster::num);
}