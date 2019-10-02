#include "stdafx.h"
#include "Title.h"
#include "Test.h"
#include"TestStage.h"
#include "TEstNPC.h"
#include "Player.h"
#include"Title_Menu.h"

Title::Title()
{
}


Title::~Title()
{
}

bool Title::Start()
{
	//m_data.CreateFromDDSTextureFromFile(L"Assets/sprite/Title.dds");
	//m_sprite.Init(m_data,1280.0f,720.0f);
	m_model.Init(L"Assets/modelData/unityChan.cmo");
	return true;
}
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonA))
	{
		NewGO<Title_Menu>(0, "Title_Menu");
		DeleteGO(this);
	}
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), {10.0f,10.0f,10.0f});

}

void Title::Draw()
{
	//m_sprite.Draw(g_graphicsEngine->GetD3DDeviceContext(), g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}