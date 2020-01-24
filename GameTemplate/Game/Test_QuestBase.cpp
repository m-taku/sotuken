#include "stdafx.h"
#include "Test_QuestBase.h"
#include"Text_Box/Text_Box.h"
#include "Stage/QuestStage.h"
#include"Player.h"
#include"Enemy/Enemy.h"
#include"Stage/Town.h"
#include"QuestManager.h"
#include"GameManager.h"


Test_QuestBase::Test_QuestBase()
{

}


Test_QuestBase::~Test_QuestBase()
{
	for (int i = 0; i < num; i++)
	{
		DeleteGO(m_text[i]);
	}
}
bool Test_QuestBase::Start()
{
	//クエストの紙のデータ
	m_srv[0].CreateFromDDSTextureFromFile(L"Assets/sprite/QuestForm.dds");
	m_sprite[0].Init(m_srv[0], 800.0f, 1000.0f);
	m_sprite[0].Update({ 0.0f,0.0f,0.0f }, CQuaternion::Identity(), CVector3::One()); 
	//文字の表示
	for (int i = 0; i < Quest::num; i++) {
		m_text[i] = NewGO<Text_Box>(10, "Tessfagn");
		m_text[i]->Init(name[i],m_Textpos[i]);
		m_text[i]->SetSpeed(0.0f);
	}
	wchar_t FileName[256];
	swprintf_s(FileName,L"Assets/sprite/Monster/%s.dds", EnglishMosterName[MonsterNo]);
	m_srv[1].CreateFromDDSTextureFromFile(FileName);
	m_sprite[1].Init(m_srv[1], 350.0f, 350.0f);
	m_sprite[1].Update({ 0.0f,150.0f,0.0f }, CQuaternion::Identity(), CVector3::One());
	m_sprite[1].SetMulColor({ 1.0f,1.0f,2.0f,1.0f });
	m_isStart = true;
	m_GameManager = FindGO<GameManager>("GameManager");
	m_time *= 60.0f;
	return true;
}
void Test_QuestBase::CreateQuest()
{
	m_doun = m_Maxdoun;
	//小型はどっちでもいいかな～～
	for (auto na : m_text)
	{
		if (na != nullptr) {
			na->SetActive(false);
		}
	}
	m_Quest = true;
	m_nowtime = 0.0f;

	//クエストに関係する減算等あればここでする。
	auto pla = FindGO<Player>("player");
	pla->TransitionState(Player::StateQuestMove);
	//DeleteGO(FindGO<Test_GuestManager>("tes"));
}
void Test_QuestBase::Update()
{
	if (m_Quest) {
		m_nowtime += GetFrameDeltaTime();
		if (m_time <= m_nowtime)
		{
			//NewGO<Test_GuestManager>(0, "tes");
			m_Quest = false;
			SetActive(false);
			m_GameManager->cheng(m_Quest);
			//DeleteGO(this);
		}
		else {
			if (m_nowdoun) {
				debugtime++;
				if (debugtime >= 120) {
					m_doun--;
					if (m_doun <= 0) {
						//NewGO<Town>(0, "town");
						//NewGO<Test_GuestManager>(0, "tes");
						m_Quest = false;
						SetActive(false);
						m_GameManager->cheng(m_Quest);
						//	DeleteGO(Stage);
						//	DeleteGO(this);
					}
					else
					{
						auto pla = FindGO<Player>("player");
						pla->TransitionState(Player::StateTownMove);
						pla->Hp = 1;

					}
					debugtime = 0;
					m_nowdoun = false;
				}
			}
		}
	}
}
void Test_QuestBase::PostDraw()
{
	if (!m_Quest) {
		for (int i = 0; i < 2; i++) {
			m_sprite[i].Draw(g_graphicsEngine->GetD3DDeviceContext());
		}
	}
}