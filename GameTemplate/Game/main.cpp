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
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	GameFont ja;
	GameObjectManager().Init(200);
	g_physics.SetDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);
	//ゲームループ。
	NewGO <Title>(0, "Title");
	while (DispatchWindowMessage() == true)
	{
		GetGameTime().Start();
		//描画開始。
		g_graphicsEngine->BegineRender();
		//ゲームパッドの更新。	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		//物理エンジンの更新。
		g_physics.Update();
		GameObjectManager().Execute();
		//カメラの更新。
		g_camera3D.Update();
		//描画終了。
		ja.BeginDraw();
		wchar_t fps[256];
		swprintf_s(fps, L"FPS = %2f", 1.0/GetFrameTime());
		ja.Draw(fps, { -FRAME_BUFFER_W /2.0f, FRAME_BUFFER_H/2.0f }, CVector4::White(), 0.0f, 1.0f, {0.0f,1.0f});
		ja.EndDraw();
		g_graphicsEngine->EndRender();
		GetGameTime().Stop();
	}
}