#include "stdafx.h"
#include "Test_QuestBase.h"


Test_QuestBase::Test_QuestBase()
{
	m_srv[0].CreateFromDDSTextureFromFile(L"Assets/sprite/Title3.dds");
	m_sprite[0].Init(m_srv[0], 600.0f, 1000.0f);
}


Test_QuestBase::~Test_QuestBase()
{
}
bool Test_QuestBase::Start()
{

	return true;
}
void Test_QuestBase::Update()
{

}