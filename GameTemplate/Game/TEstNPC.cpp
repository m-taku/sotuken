#include "stdafx.h"
#include "TEstNPC.h"
#include"Test.h"
#include "Text_Box/Text_Box.h"


TEstNPC::TEstNPC()
{
}


TEstNPC::~TEstNPC()
{
}
bool TEstNPC::Start()
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/unityChan.cmo"); 
	test =FindGO<Test>("test");
	m_bikkuri.Init(L"Assets/modelData/bikkuri.cmo");
	//m_poa.CreateMeshObject(m_model, CVector3::Zero(), CQuaternion::Identity());
	pos = { 100.0f,20.0f,-100.0f };

	//m_poa.CreateMeshObject(m_model, pos, CQuaternion::Identity());
	//m_collider.Init(10.0f, 30.0f, pos);
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
	pos = { 100.0f,100.0f,-100.0f };
	m_bikkuri.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());	
	pos = { 100.0f,20.0f,-100.0f };
	return true;
}
void TEstNPC::Update()
{
	static int i = 0;
	//posm.y -= 9.8f * 10.0f;
	//pos = m_collider.Execute(GetFrameTime(), posm);
	auto leng = pos - test->Getpos();
	if (leng.Length() <= 50.0f)
	{
		if (i == 0) {
			bikkuri = true;
			i++;
		}
	}
	else
	{
		i = 0;
		if (m_Text != NULL) {
			DeleteGO(m_Text);
			m_Text = NULL;
		}
		bikkuri = false;
	}
	if (bikkuri)
	{
		if (g_pad[0].IsTrigger(enButtonA)) {
			m_Text = NewGO<Text_Box>(10, "Text_box");
			m_Text->SetText("あいえーーーー？？？？？？？？？？？？？？");
			bikkuri = false;
		}
	}
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
}
void TEstNPC::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	if (bikkuri)
	{
		m_bikkuri.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}
}