#include "stdafx.h"
#include "Town.h"
#include"TEstNPC.h"


Town::Town()
{
}


Town::~Town()
{
}
bool Town::Init()
{
	//�n�ʂ������O�̂�
	m_Name = L"Town";
	NewGO<TEstNPC>(0, "TEstNPC");
	//���x���f�U�C���͂�����
	return true;
}