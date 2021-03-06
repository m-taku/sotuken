#include "stdafx.h"
#include "SaveData_Select.h"
#include "Text_Box/Text_Box.h"
#include "Test.h"
#include "TEstNPC.h"
#include "Player.h"
#include"TestSave.h"
#include"TestSeve_Player.h"
#include "GameManager.h"
#include"Stage/Town.h"

SaveData_Select::SaveData_Select()
{
}


SaveData_Select::~SaveData_Select()
{
}
bool SaveData_Select::Start()
{
	auto text = NewGO<Text_Box>(10, "Text_box");
	m_text.push_back(text);
	TestSeve_Player data1(1);
	auto ma = data1.GetData<PalyerData>();
	TestSeve_Player data2(2);
	auto max = data2.GetData<PalyerData>();
	TestSeve_Player data3(3);
	auto maa = data3.GetData<PalyerData>();
	text->Init(ma->name, { -400.0f,100.0f }, CVector4::White(), 0.0f, { 0.0f,0.5f });
	text = NewGO<Text_Box>(10, "Text_box");
	m_text.push_back(text);
	text->Init(max->name, { -400.0f,0.0f }, CVector4::White(), 0.0f, { 0.0f,0.5f });
	text = NewGO<Text_Box>(10, "Text_box");
	m_text.push_back(text);
	text->Init(maa->name, { -400.0f,-100.0f }, CVector4::White(), 0.0f, { 0.0f,0.5f });
	text = NewGO<Text_Box>(10, "Text_box");
	m_text.push_back(text);
	text->Init("��", { -400.0f,100.0f }, CVector4::White(), 1.0f, { 1.0f,0.5f });
	return true;
}
void SaveData_Select::Update()
{
	if (g_pad[0].IsTrigger(enButtonDown))
	{
		auto ma = m_text.begin();
		for (int i = 0; i < m_text.size() - 1; i++) {
			ma++;
		}
		auto aaa = (*ma)->Getpos();
		aaa.y = max(aaa.y - 100.0f, -100.0f);
		auto NO = No + 1;
		No = max((SaveNo)NO, SaveNo::No1);
		(*ma)->SetPos(aaa);
	}
	if (g_pad[0].IsTrigger(enButtonUp))
	{
		auto ma = m_text.begin();
		for (int i = 0; i < m_text.size() - 1; i++) {
			ma++;
		}
		auto aaa = (*ma)->Getpos();
		aaa.y = min(aaa.y + 100.0f, 100.0f);
		auto NO = No - 1;
		No = min((SaveNo)NO, SaveNo::No3);
		(*ma)->SetPos(aaa);
	}
	if (g_pad[0].IsTrigger(enButtonA))
	{

		NewGO<GameManager>(10, "GameManager");
		DeleteGO(this);
		for (auto k : m_text)
		{
			DeleteGO(k);
		}
	}

}
void SaveData_Select::Draw()
{

}