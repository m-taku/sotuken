#pragma once
class Text_Box : public IGameObject
{
public:
	Text_Box();
	~Text_Box();
	bool State();
	void Update();
	///<summary>
	///表示させる
	///</summary>
	void SetText(const std::string& ma)
	{
		char k[2] = "\n";

		int j = 0;
		for (const auto& moji : ma) {
			if (j >= 10)
			{
				m_Text += k;
				j = 0;
			}
			m_Text += moji;
			j++;
		}
	}
	///<summary>
	///
	///</summary>
	void SetPos(CVector2 pos)
	{
		m_pos = pos;
	}
	///<summary>
	///
	///</summary>
	void SetSpeed(float speed)
	{
		m_speed = speed;
	}
	void Draw();
private:
	GameFont m_font;							//文字の表示機構
	CVector2 m_pos = { -500.0f,-100.0f };		//文字の表示場所
	int m_speed = 10;							//文字の表示スピード
	CVector4 m_Coler = CVector4::White();		//文字の色
	wchar_t m_text[256];						//現在表示している文字
	std::string m_Text;							//表示する文字列
};