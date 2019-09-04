#include "stdafx.h"
#include "Test.h"


Test::Test()
{
}


Test::~Test()
{
}
bool Test::State()
{

	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/unityChan.cmo");
	return true;
}
void Test::Update()
{
	static float bai = 1.0f;
	No++;
	CQuaternion m;
	m.SetRotationDeg(CVector3::AxisZ(), 360.0f/30.0f*bai);
	m.Multiply(pos);
	if (No >= 30)
	{
		bai += 0.1;
		DeleteGO(this);
	}
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
}
void Test::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(),g_camera3D.GetProjectionMatrix());
}