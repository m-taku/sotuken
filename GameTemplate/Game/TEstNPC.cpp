#include "stdafx.h"
#include "TEstNPC.h"


TEstNPC::TEstNPC()
{
}


TEstNPC::~TEstNPC()
{
}
bool TEstNPC::State()
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/unityChan.cmo"); 
	m_bikkuri.Init(L"Assets/modelData/bikkuri.cmo");
	//m_poa.CreateMeshObject(m_model, CVector3::Zero(), CQuaternion::Identity());
	pos = { 100.0f,100.0f,-100.0f };
	m_collider.Init(10.0f, 30.0f, pos);
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
	pos = { 100.0f,100.0f,-100.0f };
	m_bikkuri.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
	return true;
}
void TEstNPC::Update()
{

	posm.y -= 9.8f * 10.0;
	pos = m_collider.Execute(GetFrameTime(), posm);
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
}
void TEstNPC::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	if (true)
	{
		m_bikkuri.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}
}