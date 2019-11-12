#include "stdafx.h"
#include "Title_Menu.h"
#include "Text_Box/Text_Box.h"
#include "Test.h"
#include"Stage/TestStage.h"
#include "TEstNPC.h"
#include "Player.h"
#include"SaveData_Select.h"
#include"Test_GuestManager.h"
#include"Stage/Town.h"

Title_Menu::Title_Menu()
{
}


Title_Menu::~Title_Menu()
{
}
bool Title_Menu::Start()
{
	//NewGO<Test_GuestManager>(0);

	auto text = NewGO<Text_Box>(10, "Text_box");
	text->Init("ÇmÇdÇvÅ@ÇfÇ`ÇlÇd", { -400.0f,100.0f }, CVector4::White(), 0.0f, { 0.0f,0.5f });
	m_text.push_back(text);
	text = NewGO<Text_Box>(10, "Text_box");
	m_text.push_back(text);
	text->Init("ÇkÇnÇ`ÇcÅ@ÇfÇ`ÇlÇd", { -400.0f,0.0f }, CVector4::White(), 0.0f, {0.0f,0.5f});
	text = NewGO<Text_Box>(10, "Text_box");
	m_text.push_back(text);
	text->Init("Å®", { -400.0f,100.0f }, CVector4::White(), 1.0f, { 1.0f,0.5f });
	return true;
}
void Title_Menu::Update()
{
	switch (m_state)
	{
	case Title_Menu::NewGame:	
		if (g_pad[0].IsTrigger(enButtonDown))
		{
			auto ma = m_text.begin();
			for (int i = 0; i < m_text.size() - 1; i++) {
				ma++;
			}
			(*ma)->SetPos({ -400.0f,0.0f });
			m_state = LoadGame;
			break;
		}
		if (g_pad[0].IsTrigger(enButtonA))
		{
			NewGO<Town>(0,"town");
			NewGO<Test_GuestManager>(0,"tes");
			NewGO<Player>(0, "player")->SetPosition({-400.0f,500.0f,0.0f});
			DeleteGO(this);
			for (auto k : m_text)
			{
				DeleteGO(k);
			}
			break;
		}


	case Title_Menu::LoadGame:
		if (g_pad[0].IsTrigger(enButtonUp))
		{
			auto ma = m_text.begin();
			for (int i = 0; i < m_text.size() - 1; i++) {
				ma++;
			}
			(*ma)->SetPos({ -400.0f,100.0f });
			m_state = NewGame;
			break;
		}
		if (g_pad[0].IsTrigger(enButtonA))
		{
			NewGO<SaveData_Select>(0, "SaveData_Select");
			DeleteGO(this);
			for (auto k : m_text)
			{
				DeleteGO(k);
			}
			break;
		}
		break;
	default:
		break;
	}



}
void Title_Menu::Draw()
{

}