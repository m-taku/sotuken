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
	m_characon.Init(
		20.0f,
		100.0f,
		m_position);
	return true;

}
void Quest_NPC::Talk()
{
	isupdate = true;
	switch (m_TalkChange)
	{
	case Quest_NPC::TalkStart:
		if (QuestManager::WaitQuest != m_QuestManager->GetOrderMode()) {
			if (g_pad[0].IsTrigger(enButtonA) && m_Text[0] == NULL) {
				m_Text[0] = NewGO<Text_Box>(10, "Text_box");
				switch (m_TalkState)
				{
				case QuestSelect:
					m_Text[0]->SetText("クエストを受けますか？");
					m_Text[0]->SetSpeed(0);
					break;
				case QuestCancel:
					m_Text[0]->SetText("クエストをキャンセルする？");
					m_Text[0]->SetSpeed(0);
					m_QuestManager->ChangeOrderMode(QuestManager::Wait);
					m_TalkChange = PrintQuest;
					break;
				default:
					break;
				}
				m_player->TransitionState(StateWate);
			}
			else if (m_Text[0] != NULL)
			{
				if (m_Text[0]->Getend()) {
					m_Text[1] = NewGO<Text_Box>(10, "Text_box");
					m_Text[1]->SetPos({ -500.0f, -130.0f });
					m_Text[1]->SetText("はい　いいえ");
					m_Text[1]->SetSpeed(0);
					m_TalkChange = PrintQuest;
				}
			}
		}
		break;
	case PrintQuest:
		switch (m_TalkState)
		{
		case QuestSelect:	
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
			break;
		case QuestCancel:
			if (g_pad[0].IsTrigger(enButtonA)|| g_pad[0].IsTrigger(enButtonB))
			{
				m_QuestManager->SetActive(true);
				m_QuestManager->Printkami();
				for (int j = 0; j < 2; j++) {
					if (m_Text[j] != NULL) {
						DeleteGO(m_Text[j]);
						m_Text[j] = NULL;
					}
				}
				m_QuestManager->ChangeOrderMode(QuestManager::Cancel);
				m_TalkChange = TalkWate;
			}
			break;
		default:
			break;
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
	m_addGravityTime += 1.0f*GetFrameDeltaTime();
	m_fallSpeed = (GRAVITY_PARAM*pow(m_addGravityTime, 2.0f)) * 0.5f;
	m_movespeed.y -= m_fallSpeed;
	if (m_characon.IsOnGround())
	{
		m_addGravityTime = 0.0f;
		m_fallSpeed = 0.0f;
		m_movespeed.y = 0.0f;
	}
	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	m_model.UpdateWorldMatrix(m_position, CQuaternion::Identity(), CVector3::One());
	m_model.Draw(enNormal, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	isupdate = false;
}