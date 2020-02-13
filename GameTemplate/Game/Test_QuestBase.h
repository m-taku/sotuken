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
	char JapanMosterName[mosternum][256] = { "ドラゴメス", "モンスター2","モンスター3" };
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
		int BicBos_syurui;				//大型の種類			
		int kogata_pos[256];			//大型の敵のPOS
		int smorlBos_syurui;			//小型の種類
		int oogata_pos[256];			//小型の敵のPOS
	};
	struct settei
	{
		int jikan;				//制限時間
		int daunnsuu;			//残機数
		int hoysyuu;			//報酬金
	};
	struct staji
	{
		int tikeidata;			//マップデータ
		int doropuItem;			//ドロップアイテム（採取）
	};
};
