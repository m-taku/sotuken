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
	///�\��������
	///�S�p�����݂̂ɂ��Ă�������
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
	GameFont m_font;							//�����̕\���@�\
	CVector4 m_Coler = CVector4::White();		//�����̐F
	CVector2 m_pos = { -500.0f,-100.0f };		//�����̕\���ꏊ
	CVector2 m_pivot = { 0.5f,0.5f };			//������̃s�{�b�g
	std::string m_Text;							//�\�����镶����
	wchar_t m_text_now[256];					//���ݕ\�����Ă��镶��
	bool m_end = false;							//�����\�����I��������
	int m_speed = 10;							//�����̕\���X�s�[�h
	int m_count = 0;							//�����̌�
};