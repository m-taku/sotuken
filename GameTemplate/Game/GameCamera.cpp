#include "stdafx.h"
#include "GameCamera.h"

namespace smEngine {
	GameCamera::GameCamera()
	{
	}

	GameCamera::~GameCamera()
	{
	}

	void GameCamera::Init()
	{
		m_target = CVector3::Zero();
		m_position = CVector3::Back()*MAXDIST;
		g_camera3D.SetFar(5000.0f);
		g_camera3D.SetPosition(g_camera3D.GetPosition());
		g_camera3D.SetTarget(g_camera3D.GetTarget());
		g_camera3D.SetUp(CVector3::Up());
	}

	void GameCamera::Update()
	{
		g_camera3D.Update();
	}
}

