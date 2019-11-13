#include "stdafx.h"
#include "TestStage.h"

#include"TEstNPC.h"

TestStage::TestStage()
{
}


TestStage::~TestStage()
{
}
bool TestStage::Init()
{
	NewGO<TEstNPC>(0,"TEstNPC");
	//m_Name = L"Assets/modelData/jiman.cmo";
	//レベルデザインはここで
	return true;
}