#include "stdafx.h"
#include "QuestResult.h"

#include"GameManager.h"

QuestResult::QuestResult()
{
}


QuestResult::~QuestResult()
{
	for (int i = 0; i < 4; i++)
	{
		DeleteGO(m_text[i]);
	}
}
bool QuestResult::Start()
{
	//音とかどうっすか
	//ChengAllActive(false);
	for (int i = 0; i < 4; i++) {
		m_text[i] = NewGO<Text_Box>(10, "ResultText_Box");
	}
	if (IsClear)
	{
		m_text[0]->SetText("Clear");
		m_text[1]->SetText("クリアタイム");
		char mane[256];
		sprintf(mane, "%d分%.2f秒",10,0.5f);
		m_text[2]->SetText(mane);
		m_text[3]->SetText("Clear");
	}
	else
	{
		m_text[0]->SetText("End");
		m_text[1]->SetText("タイム");
		char mane[256];
		sprintf(mane, "%d分%.2f秒", 10, 0.5f);
		m_text[2]->SetText(mane);
		m_text[3]->SetText("pures A");
	}
	m_isActive = true;
	return true;
}
void QuestResult::Update()
{
	m_nowtime += GetFrameDeltaTime();
	if (10.0f >= m_nowtime)
	{
		if (m_furag) {
			g_graphicsEngine->GetFade()->FadeOutStart();
			m_furag = false;
		}
		else {
			if (!g_graphicsEngine->GetFade()->IsInFade()) {

				FindGO<GameManager>("GameManager")->ChangeNotify(GameManager::EndQuest);
				DeleteGO(this);
			}
		}
	}
}
