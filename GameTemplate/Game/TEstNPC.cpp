#include "stdafx.h"
#include "TEstNPC.h"
#include"Test.h"
#include "Text_Box/Text_Box.h"
#include "Player.h"
#include"QuestManager.h"
#include"graphics/SkinModel.h"
#include"Test_GuestManager.h"


TEstNPC::TEstNPC()
{
}


TEstNPC::~TEstNPC()
{
}
bool TEstNPC::Start()
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/Dragon_1.cmo"); 
	m_model.EnableShadowCaster(true);
	test =FindGO<Player>("player");
	m_bikkuri.Init(L"Assets/modelData/bikkuri.cmo");
	m_collider.Init(10.0f, 30.0f, pos);
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
	CVector3 posa = { pos.x,pos.y + 100.0f,pos.z };
	m_bikkuri.UpdateWorldMatrix(posa, CQuaternion::Identity(), CVector3::One());
	return true;
}
void TEstNPC::Update()
{
	static int i = 0;
	posm.y -= 9.8f * 1.0f;
	pos = m_collider.Execute(GetFrameDeltaTime(), posm);
	auto leng = pos - test->GetPosition();
	if (leng.Length() <= 100.0f)
	{
		if (i == 0) {
			bikkuri = true;
			i++;
		}
	}
	else
	{
		i = 0;
		for (int j = 0; j < 2; j++) {
			if (m_Text[j] != NULL) {
				DeleteGO(m_Text[j]);
				m_Text[j] = NULL;
			}
		}
		bikkuri = false;
	}
	if (bikkuri)
	{
		if (g_pad[0].IsTrigger(enButtonB) && m_Text[0] == NULL) {
			m_Text[0] = NewGO<Text_Box>(10, "Text_box");
			m_Text[0]->SetText("クエストを受けますか？");
			m_Text[0]->SetSpeed(2);
			test->TransitionState(Player::StateWate);
		}
		if (m_Text[0] != NULL && m_Text[1] == NULL) {
			if (m_Text[0]->Getend() && m_Text[1] == NULL)
			{
				m_Text[1] = NewGO<Text_Box>(10, "Text_box");
				m_Text[1]->SetPos({ -500.0f, -130.0f });
				m_Text[1]->SetText("はい　いいえ");
				m_Text[1]->SetSpeed(0);
			}
		}
		if (m_Text[1] != NULL)
		{
			if (g_pad[0].IsTrigger(enButtonB))
			{
				NewGO<QuestManager>(0,"da");
				for (int j = 0; j < 2; j++) {
					if (m_Text[j] != NULL) {
						DeleteGO(m_Text[j]);
						m_Text[j] = NULL;
					}
				}
				
				bikkuri = false;
			}
			else if(g_pad[0].IsTrigger(enButtonX))
			{
				for (int j = 0; j < 2; j++) {
					if (m_Text[j] != NULL) {
						DeleteGO(m_Text[j]);
						m_Text[j] = NULL;
					}
				}

				test->TransitionState(Player::StateTownMove);
				bikkuri = false;
			}
		}
	}
	CVector3 posa = { pos.x,pos.y + 100.0f,pos.z };
	m_bikkuri.UpdateWorldMatrix(posa, CQuaternion::Identity(), CVector3::One());
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
}
void TEstNPC::Draw()
{
	m_model.Draw(enNormal, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	if (bikkuri)
	{
		m_bikkuri.Draw(enNormal, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}
}
