#include "stdafx.h"
#include "HitObject.h"


HitObject::HitObject()
{
}


HitObject::~HitObject()
{
}


void HitObject::Create(const CVector3* pos, float radius, std::function<void(float damage)>  objict, objict_Name name)
{
	//各設定の初期化
	m_pos = pos;
	m_radius = radius;
	m_fuk = objict;
	CVector3 pos1;
	pos1.Length();
	m_name = name;
}
bool HitObject::HitTest(CVector3 pos, float Circle, float damage)
{
	auto rengs = *m_pos - pos;		//球体で判定（aabb等に変更する可能性あり）
	if (rengs.Length() <= m_radius + Circle)
	{
		//当たったので登録されている関数を呼ぶ
		m_fuk(damage);
		return true;
	}
	//当たってないのでfalse
	return false;
}