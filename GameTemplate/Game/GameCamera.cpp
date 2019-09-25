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
		m_position = CVector3::Zero();
		m_target = CVector3::Back()*MAXDIST;
		g_camera3D.SetPosition(m_position);
		g_camera3D.SetTarget(m_target);
		g_camera3D.SetUp(CVector3::Up());
	}

	void GameCamera::Update()
	{
		g_camera3D.SetPosition(m_position);
		g_camera3D.SetTarget(m_target);
		g_camera3D.SetUp(CVector3::Up());
	}
}

