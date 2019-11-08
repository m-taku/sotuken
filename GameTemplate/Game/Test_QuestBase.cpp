#include "stdafx.h"
#include "Test_QuestBase.h"
#include"Text_Box/Text_Box.h"


Test_QuestBase::Test_QuestBase()
{

}


Test_QuestBase::~Test_QuestBase()
{
}
bool Test_QuestBase::Start()
{
	//クエストの紙のデータ
	m_srv[0].CreateFromDDSTextureFromFile(L"Assets/sprite/QuestForm.dds");
	m_sprite[0].Init(m_srv[0], 800.0f, 1000.0f);
	m_sprite[0].Update({ 0.0f,0.0f,0.0f }, CQuaternion::Identity(), CVector3::One()); 
	//文字の表示
	for (int i = 0; i < Quest::num; i++) {
		m_text[i] = NewGO<Text_Box>(10, "Tessfagn");
		m_text[i]->Init(name[i],m_Textpos[i]);
		m_text[i]->SetSpeed(0.0f);
	}
	wchar_t FileName[256];
	swprintf_s(FileName,L"Assets/sprite/Monster/%s.dds", EnglishMosterName[MonsterNo]);
	m_srv[1].CreateFromDDSTextureFromFile(FileName);
	m_sprite[1].Init(m_srv[1], 350.0f, 350.0f);
	m_sprite[1].Update({ 0.0f,150.0f,0.0f }, CQuaternion::Identity(), CVector3::One());
	m_sprite[1].SetMulColor({ 1.0f,1.0f,2.0f,1.0f });
	m_isStart = true;
	return true;
}
void Test_QuestBase::PostDraw()
{
	for (int i = 0; i < 2; i++) {
		m_sprite[i].Draw(g_graphicsEngine->GetD3DDeviceContext());
	}
}