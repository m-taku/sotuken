#pragma once
#include"Text_Box/Text_Box.h"
class Enemy;
class GameManager;
class Test_QuestBase : public IGameObject
{
public:
	Test_QuestBase();
	~Test_QuestBase();
	enum Quest
	{
		titor,
		moster,
		kane,
		time,
		doun,
		iraisya,
		bikou,
		num
	};
	enum Moster
	{
		monnsu,
		monnsu2,
		monnsu3,
		mosternum
	};
	bool Start();
	void Update();
	void TextureDraw();
	void CreateQuest();
	void ChangePaper()
	{
		m_isActive = !m_isActive;
		for (auto na : m_text)
		{
			if (na != nullptr) {
				na->SetActive(m_isActive);
			}
		}
	}
	void Playerdoun()
	{
		g_graphicsEngine->GetFade()->FadeOutStart();
		m_nowdoun = true;
	}
	float GetMAXTime()
	{
		return m_time;
	}
	float GetNowTime()
	{
		return m_nowtime;
	}
protected:
	char  name[num][256] = { NULL };
	Text_Box* m_text[num] = { nullptr };
	int m_Maxdoun = 0;
	float m_time = FLT_MAX;
	Moster MonsterNo = mosternum;
	wchar_t EnglishMosterName[mosternum][256] = {L"monnsu",L"monnsu2", L"monnsu3"};
	char JapanMosterName[mosternum][256] = { "�h���S���X", "�����X�^�[2","�����X�^�[3" };
private:
	Sprite m_sprite[2];
	ShaderResourceView m_srv[2];
	void* monsu = nullptr;
	void ChengResult(bool flag);
	bool m_Quest = false;
	bool m_nowdoun = false;
	int m_doun = 0;
	
	float m_nowtime = 0.0f;
	Enemy* m_target = nullptr;
	float debugtime = 0.0f;

	CVector2 m_Textpos[num] = {
		{0.0f,400.0f},
	{ 0.0f,-120.0f },
	{ -180.0f,-200.0f },
	{ 180.0f,-200.0f },
	{ 0.0f,-280.0f },
	{ -200.0f,-380.0f },
	{ 0.0f,-420.0f }
	};
};
struct ma{
	struct taki
	{
		int BicBos_syurui;				//��^�̎��			
		int kogata_pos[256];			//��^�̓G��POS
		int smorlBos_syurui;			//���^�̎��
		int oogata_pos[256];			//���^�̓G��POS
	};
	struct settei
	{
		int jikan;				//��������
		int daunnsuu;			//�c�@��
		int hoysyuu;			//��V��
	};
	struct staji
	{
		int tikeidata;			//�}�b�v�f�[�^
		int doropuItem;			//�h���b�v�A�C�e���i�̎�j
	};
};
