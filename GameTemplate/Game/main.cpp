#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include"Title.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{

	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	//カメラを初期化。
	g_camera3D.SetPosition({ 0.0f, 100.0f, -600.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	GameFont FPS;
	GameObjectManager().Init(20);
	g_physics.SetDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);
	//ゲームループ。
	NewGO<Title>(0, "Title");

	int innn = 0;
	int count = 0;
	while (DispatchWindowMessage() == true)
	{
		GetGameTime().Start();
		GetSkinModelManager().SetNo(innn);
		//描画開始。
		g_graphicsEngine->BegineRender();
		//ゲームパッドの更新。	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		//物理エンジンの更新。
		g_physics.Update();
		GameObjectManager().Execute();
		//描画終了。
		FPS.BeginDraw();
		wchar_t fps[256];
		swprintf_s(fps, L"FPS = %2f", 1.0/ GetFrameDeltaTime());
		FPS.Draw(fps, { -FRAME_BUFFER_W /2.0f, FRAME_BUFFER_H/2.0f }, CVector4::White(), 0.0f, 1.0f, {0.0f,1.0f});
		FPS.EndDraw();
		g_graphicsEngine->EndRender();
		innn++;
		innn %= 2;
		count++;
		GetGameTime().Stop();
	}
	delete g_graphicsEngine;
}