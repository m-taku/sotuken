#include "stdafx.h"
#include "Mikyan.h"


Mikyan::Mikyan( const wchar_t* modelName, CVector3 pos, CQuaternion rot)
{
	m_skinModel.Init(modelName);
	m_pos = pos;
	m_rot = rot;
}


Mikyan::~Mikyan()
{
}
void Mikyan::Update()
{
	angle += 0.1f;
	m_rot.SetRotation(CVector3::AxisY(), angle);
	m_skinModel.UpdateWorldMatrix(m_pos, m_rot, CVector3::One());
}
void Mikyan::Draw()
{
	m_skinModel.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}
