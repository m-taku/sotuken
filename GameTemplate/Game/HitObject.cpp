#include "stdafx.h"
#include "HitObject.h"


HitObject::HitObject()
{
}


HitObject::~HitObject()
{
}


void HitObject::Create(const CVector3* pos, float radius, std::function<void(float damage, CVector3 ObjDate)>  objict, objict_Name name)
{
	//�e�ݒ�̏�����
	m_pos = pos;
	m_radius = radius;
	m_fuk = objict;
	CVector3 pos1;
	pos1.Length();
	m_name = name;
}
bool HitObject::HitTest(CVector3 originpos,CVector3 pos, float Circle, float damage)
{
	auto pos2 = *m_pos;
	//�Œ�̈ʒu�ŏグ��
	pos2.y = 50.0f;
	auto rengs = pos2 - pos;		//���̂Ŕ���iaabb���ɕύX����\������j
	if (rengs.Length() <= m_radius + Circle)
	{
		//���������̂œo�^����Ă���֐����Ă�
		m_fuk(damage, pos);
		return true;
	}
	//�������ĂȂ��̂�false
	return false;
}