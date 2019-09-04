#include "stdafx.h"
#include "system/system.h"
#include "level/Level.h"
#include "Test.h"
#include"GameFont.h"


///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{

	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	GameFont ja;
	GameObjectManager().Init(200);
	//�Q�[�����[�v�B
	int no = 0;
	while (DispatchWindowMessage() == true)
	{
		GetGameTime().Start();
		if (++no >= 10) 
		{
			NewGO<Test>(0, "nerklangla");
			no = 0;
		}
		//�`��J�n�B
		g_graphicsEngine->BegineRender();
		//�Q�[���p�b�h�̍X�V�B	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		//�����G���W���̍X�V�B
		g_physics.Update();
		GameObjectManager().Execute();
		//�J�����̍X�V�B
		g_camera3D.Update();
		//�`��I���B
		ja.BeginDraw();
		wchar_t fps[256];
		swprintf_s(fps, L"FPS = %2f", 1.0/GetFrameTime());
		ja.Draw(fps, CVector2::Zero(), CVector4::White(),0.0f,1.0f);
		ja.EndDraw();
		g_graphicsEngine->EndRender();
		GetGameTime().Stop();
	}
}