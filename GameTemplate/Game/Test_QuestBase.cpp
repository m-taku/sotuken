#include "stdafx.h"
#include "Test_QuestBase.h"


Test_QuestBase::Test_QuestBase()
{
	m_srv[0].CreateFromDDSTextureFromFile(L"Assets/sprite/Title.dds");
	m_sprite[0].Init(m_srv[0], 800.0f, 500.0f);
	m_sprite[0].Update({0.0f,0.0f,0.0f},CQuaternion::Identity(),CVector3::One());
	m_srv[1].CreateFromDDSTextureFromFile(L"Assets/sprite/Title.dds");
	m_sprite[1].Init(m_srv[1], 800.0f-100.0f, 500.0f-80.0f);
	m_sprite[1].Update({ 0.0f,0.0f,0.0f }, CQuaternion::Identity(), CVector3::One());
	m_sprite[1].SetMulColor({ 1.0f,1.0f,2.0f,1.0f });
}


Test_QuestBase::~Test_QuestBase()
{
}
bool Test_QuestBase::Start()
{

	return true;
}
void Test_QuestBase::Draw()
{
	for (int i = 0; i < 2; i++) {
		m_sprite[i].Draw(g_graphicsEngine->GetD3DDeviceContext());
	}
}