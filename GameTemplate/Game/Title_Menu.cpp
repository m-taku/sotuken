#include "stdafx.h"
#include "Title_Menu.h"
#include "Text_Box/Text_Box.h"
#include "Test.h"
#include"TestStage.h"
#include "TEstNPC.h"
#include "Player.h"


Title_Menu::Title_Menu()
{
}


Title_Menu::~Title_Menu()
{
}
bool Title_Menu::Start()
{

	auto text = NewGO<Text_Box>(10, "Text_box");
	m_text.push_back(text);
	text->SetText("‚m‚d‚v@‚f‚`‚l‚d");
	text->SetPos({ -200.0f,100.0f });
	text->SetSpeed(0.0f);
	text = NewGO<Text_Box>(10, "Text_box");
	m_text.push_back(text);
	text->SetText("‚k‚n‚`‚c@‚f‚`‚l‚d");
	text->SetSpeed(0.1f);
	text->SetPos({ -200.0f,000.0f });
	return true;
}
void Title_Menu::Update()
{
	if (g_pad[0].IsTrigger(enButtonA))
	{
		NewGO<Test>(0, "test");
		NewGO<TestStage>(0, "jaio");
		NewGO<TEstNPC>(0, "TEstNPC");
		NewGO<Player>(0, "player");
		DeleteGO(this);
		for (auto k : m_text)
		{
			DeleteGO(k);
		}
	}
}
void Title_Menu::Draw()
{

}