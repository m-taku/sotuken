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
	No++;
	pos.x += 10.0f;
	if (No >= 10)
	{
		DeleteGO(this);
	}
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
}
void Test::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(),g_camera3D.GetProjectionMatrix());
}