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
	//地面だけ名前のみ
	m_Name = L"Town";
	NewGO<TEstNPC>(0, "TEstNPC");
	//レベルデザインはここで
	return true;
}