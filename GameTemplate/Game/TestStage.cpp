#include "stdafx.h"
#include "TestStage.h"


TestStage::TestStage()
{
}


TestStage::~TestStage()
{
}
bool TestStage::Start()
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/jiman.cmo");
	m_poa.CreateMeshObject(m_model, CVector3::Zero(), CQuaternion::Identity());
	//m_collider.Init(100.0f, 30.0f, pos);
	return true;
}
void TestStage::Update()
{
	static float bai = 0.0f;
	No++;
	if (No >= 100)
	{
		bai *= -1.0f;
		No = 0;
	}
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
}
void TestStage::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}