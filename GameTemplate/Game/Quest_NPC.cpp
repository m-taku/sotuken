#include "stdafx.h"
#include "Quest_NPC.h"
#include "Text_Box/Text_Box.h"
#include"Player.h"
#include"QuestManager.h"


Quest_NPC::Quest_NPC()
{
}			

Quest_NPC::~Quest_NPC()
{
}
bool Quest_NPC::Init()
{
	m_QuestManager = FindGO<QuestManager>("QuestManager");
	m_model.Init(L"Assets/modelData/unityChan.cmo");
	m_model.EnableShadowCaster(true);
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	m_bikkuri.Init(L"Assets/modelData/bikkuri.cmo");
	CVector3 posa = { m_position.x,m_position.y + 100.0f,m_position.z };
	m_bikkuri.UpdateWorldMatrix(posa, CQuaternion::Identity(), CVector3::One());
	return true;

}
void Quest_NPC::Talk()
{
	isupdate = true;
	switch (m_TalkChange)
	{
	case Quest_NPC::TalkStart:
		if (g_pad[0].IsTrigger(enButtonA)&& m_Text[0] == NULL) {
			m_Text[0] = NewGO<Text_Box>(10, "Text_box");
			m_Text[0]->SetText("クエストを受けますか？");
			m_Text[0]->SetSpeed(2);
			m_player->TransitionState(StateWate);
		}	
		else if (m_Text[0]!=NULL)
		{
			if (m_Text[0]->Getend()) {
				m_Text[1] = NewGO<Text_Box>(10, "Text_box");
				m_Text[1]->SetPos({ -500.0f, -130.0f });
				m_Text[1]->SetText("はい　いいえ");
				m_Text[1]->SetSpeed(0);
				m_TalkChange = PrintQuest;
			}
		}
		else
		{
		}
		break;
	case PrintQuest:
		if (g_pad[0].IsTrigger(enButtonA))
		{
			m_QuestManager->SetActive(true);
			m_QuestManager->Printkami();
			for (int j = 0; j < 2; j++) {
				if (m_Text[j] != NULL) {
					DeleteGO(m_Text[j]);
					m_Text[j] = NULL;
				}
			}
			switch (m_TalkState)
			{
			case QuestSelect:
				break;
			case QuestCancel:
				m_QuestManager->ChangeOrderMode(QuestManager::Cancel);
				break;
			default:
				break;
			}
			m_TalkChange = TalkWate;
		}
		else if (g_pad[0].IsTrigger(enButtonB))
		{
			for (int j = 0; j < 2; j++) {
				if (m_Text[j] != NULL) {
					DeleteGO(m_Text[j]);
					m_Text[j] = NULL;
				}
			}
			m_player->TransitionState(StateTownMove);
			m_TalkChange = TalkStart;
		}
	case TalkWate:

		break;
	case TalkEnd:
		//なんかいいたことあれば書けば？？？
		m_TalkChange = TalkStart;
		break;
	default:
		break;
	}
}
void Quest_NPC::Draw()
{
	if (m_Text[0] == NULL && isupdate == true)
	{
		m_bikkuri.Draw(enNormal, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}
	m_model.Draw(enNormal, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	isupdate = false;
}