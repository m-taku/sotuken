#include "stdafx.h"
#include "Test.h"


Test::Test()
{
}


Test::~Test()
{
}
bool Test::Start()
{

	m_collider.Init(50.0f, 30.0f, pos);
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/unityChan.cmo");
	return true;
}
void Test::Update()
{
	/*posm.x = g_pad[0].GetLStickXF() * 100.0f;
	posm.z = g_pad[0].GetLStickYF() * 100.0f;*/
	//posm.x = 300.0f;
	//posm.x *= bai;

	posm.y -= 9.8f * 10.0f;
	pos = m_collider.Execute(GetFrameDeltaTime(),posm);
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
}
void Test::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(),g_camera3D.GetProjectionMatrix());
}