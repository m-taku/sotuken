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
	m_model.Init(L"Assets/modelData/unityChan.cmo");
	m_srv.CreateFromDDSTextureFromFile(L"Assets/sprite/utc_all2.dds");
	m_sprite.Init(m_srv, 500.0f, 500.0f);
	return true;
}
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonA))
	{
		NewGO<Title_Menu>(0, "Title_Menu");
		DeleteGO(this);
	}
	//pos.z -= 1.0f;
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), { 10.0f,10.0f,10.0f });

}

void Title::Draw()
{
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	m_sprite.Draw(d3dDeviceContext);
	//m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}