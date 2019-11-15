#include "stdafx.h"
#include "Title.h"
#include "Test.h"
#include "TEstNPC.h"
#include "Player.h"
#include"Title_Menu.h"
#include"Text_Box/Text_Box.h"

Title::Title()
{
}


Title::~Title()
{
}

bool Title::Start()
{
	m_model.Init(L"Assets/modelData/unityChan.cmo");
	m_srv.CreateFromDDSTextureFromFile(L"Assets/sprite/Title.dds");	
	m_sprite.Init(m_srv, FRAME_BUFFER_W, FRAME_BUFFER_H);
	text = NewGO<Text_Box>(10, "Text_box");
	text->Init("ÇoÇíÇÖÇìÇìÅ@Ç`Å@ÇaÇïÇîÇîÇèÇé", { 0.0f,-300.0f }, CVector4::White(), 0.0f, { 0.5f,0.5f });

	return true;
}
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonA))
	{
		NewGO<Title_Menu>(0, "Title_Menu");
		DeleteGO(text);
		DeleteGO(this);
	}
	//pos.z -= 1.0f;
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), { 1.0f,1.0f,10.0f });

}
void Title::PostDraw()
{
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	m_sprite.Draw(d3dDeviceContext);
	//m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}
void Title::Draw()
{
	//ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//m_sprite.Draw(d3dDeviceContext);
	//m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}