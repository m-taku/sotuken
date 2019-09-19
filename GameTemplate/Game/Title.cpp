#include "stdafx.h"
#include "Title.h"
#include "Test.h"
#include"TestStage.h"
#include "TEstNPC.h"
#include "Player.h"

Title::Title()
{
}


Title::~Title()
{
}

bool Title::Start()
{

	m_model.Init(L"Assets/modelData/unityChan.cmo");
	return true;
}

void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonA))
	{
		NewGO<Test>(0, "test");
		NewGO<TestStage>(0, "jaio");
		NewGO<TEstNPC>(0, "TEstNPC");
		NewGO<Player>(0, "player");
		DeleteGO(this);
	}
	//pos.z -= 1.0f;
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), {10.0f,10.0f,10.0f});

}

void Title::Draw()
{
	m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}