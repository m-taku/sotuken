#include "stdafx.h"
#include "Text_Box.h"


Text_Box::Text_Box()
{
}


Text_Box::~Text_Box()
{
}
bool Text_Box::State()
{

	return true;
}
void Text_Box::Update()
{
	
	count += m_Text.length();
	setlocale(LC_ALL, "japanese");
	size_t kosuu = 0;
	mbstowcs_s(&kosuu, m_text_now, (int)(min(count / m_speed+1, m_Text.length())), m_Text.c_str(), _TRUNCATE);
}
void Text_Box::Draw()
{
	m_font.BeginDraw();
	//swprintf_s(m_text_now, L"%s",  m_Text.c_str());
	m_font.Draw(m_text_now, m_pos, m_Coler,0.0f,0.5f, m_pivot);
	m_font.EndDraw();
}