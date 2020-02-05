#pragma once
class Text_Box : public IGameObject
{
public:
	Text_Box();
	~Text_Box();
	bool State();
	void Update();
	void Init(const std::string& text, CVector2 pos = CVector2::Zero(), CVector4 color = CVector4::White(), float speed = 1.0f, CVector2 pivot = { 0.5f,0.5f })
	{
		State();
		SetText(text);
		SetPos(pos);
		m_Coler = color;
		SetSpeed(speed);
		m_pivot = pivot;
	}
	///<summary>
	///表示させる
	///全角文字のみにしてください
	///</summary>
	void SetText(const std::string& ma)
	{
		char k[2] = "\n";

		int j = 0;
		for (const auto& moji : ma) {
			if (moji != NULL) {
				if (j >= 200)
				{
					m_Text += k;
					j = 0;
				}
				m_Text += moji;
				j++;
			}
			else
			{
				break;
			}
		}
		m_Text += "\0";
		m_speed = (int)m_Text.length();
		m_count = 0;
	}
	bool Getend()
	{
		return m_end;
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
		if (speed > 0) {
			m_speed /= speed;
			if (m_speed <= 0) {
				m_speed = 1;
			}
		}
		else
		{
			m_speed = 1;
		}
	}
	const CVector2& Getpos()
	{
		return m_pos;
	}
	void UIDraw();
private:
	GameFont m_font;							//文字の表示機構
	CVector4 m_Coler = CVector4::White();		//文字の色
	CVector2 m_pos = { -500.0f,-100.0f };		//文字の表示場所
	CVector2 m_pivot = { 0.5f,0.5f };			//文字列のピボット
	std::string m_Text;							//表示する文字列
	wchar_t m_text_now[256];					//現在表示している文字
	bool m_end = false;							//文字表示が終了したか
	int m_speed = 10;							//文字の表示スピード
	int m_count = 0;							//文字の個数
};