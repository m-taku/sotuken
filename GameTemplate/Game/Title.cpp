#include "stdafx.h"
#include "Title.h"
#include "Test.h"
#include"TestStage.h"
#include "TEstNPC.h"

Title::Title()
{
}


Title::~Title()
{
}

bool Title::State()
{

	m_model.Init(L"Assets/modelData/unityChan.cmo");
	return true;
}

void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonA))
	{
		NewGO<Test>(0, "nerklangla");
		NewGO<TestStage>(0, "jaio");
		NewGO<TEstNPC>(0, "TEstNPC");
		DeleteGO(this);
	}
	//pos.z -= 1.0f;
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), {10.0f,10.0f,10.0f});

}

void Title::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}