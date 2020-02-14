#include "stdafx.h"
#include "Title.h"
#include "Player.h"
#include"Title_Menu.h"
#include "GameManager.h"
#include"Text_Box/Text_Box.h"

Title::Title()
{
}


Title::~Title()
{
}

bool Title::Start()
{
	m_srv.CreateFromDDSTextureFromFile(L"Assets/sprite/Title5.dds");
	m_sprite.Init(m_srv, FRAME_BUFFER_W, FRAME_BUFFER_H);
	text = NewGO<Text_Box>(10, "Text_box");
	text->Init("Ｐｒｅｓｓ　Ａ　Ｂｕｔｔｏｎ", { 0.0f,-300.0f }, CVector4::White(), 0.0f, { 0.5f,0.5f });
	m_pSoundSource = new CSoundSource;
	m_pSoundSource->Init(L"Assets/sound/Op.wav", "TitleBGM");
	m_pSoundSource->Play(true);
	return true;
}
void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonA))
	{
		m_furag = true;

		g_graphicsEngine->GetFade()->FadeOutStart();
	}
	if (m_furag) {
		if (!g_graphicsEngine->GetFade()->IsInFade())
		{
			m_pSoundSource->Stop();
			//m_pSoundSource->Release();
			delete m_pSoundSource;
			//セーブ関係入れたらいけるよーーー
			//NewGO<Title_Menu>(0, "Title_Menu");

			NewGO<GameManager>(10, "GameManager");
			DeleteGO(text);
			DeleteGO(this);
		}
	}
}
void Title::TextureDraw()
{
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	m_sprite.Draw(d3dDeviceContext);
}
void Title::Draw()
{
	//ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//m_sprite.Draw(d3dDeviceContext);
	//m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}