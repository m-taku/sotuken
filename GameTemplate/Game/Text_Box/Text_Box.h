#pragma once
class Text_Box : public IGameObject
{
public:
	Text_Box();
	~Text_Box();
	bool State();
	void Update();
	///<summary>
	///�\��������
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
	GameFont m_font;							//�����̕\���@�\
	CVector2 m_pos = { -500.0f,-100.0f };		//�����̕\���ꏊ
	int m_speed = 10;							//�����̕\���X�s�[�h
	CVector4 m_Coler = CVector4::White();		//�����̐F
	wchar_t m_text[256];						//���ݕ\�����Ă��镶��
	std::string m_Text;							//�\�����镶����
};