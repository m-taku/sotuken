#include "stdafx.h"
#include "Text_box.h"


Text_box::Text_box()
{
}


Text_box::~Text_box()
{
}
bool Text_box::State()
{

	return true;
}
void Text_box::Update()
{
	static int count = 0;
	count++;
	setlocale(LC_ALL, "japanese");
	size_t kosuu = 0;
	mbstowcs_s(&kosuu, m_text, min(count/ m_speed +1, m_Text.length()), m_Text.c_str(), _TRUNCATE);
}
void Text_box::Draw()
{
	m_font.BeginDraw();
	m_font.Draw(m_text, m_pos, m_Coler,0.0f,0.5f);
	m_font.EndDraw();
}