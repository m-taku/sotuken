#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
	m_characon.Init(
		50.0f,
		80.0f,
		m_position
	);
}


Enemy::~Enemy()
{
}
bool Enemy::Start()
{
	m_skinmodel.Init(L"Assets/modelData/unityChan.cmo");
	m_skinmodel.EnableShadowCaster(true);
	return true;
}
void Enemy::Update()
{
	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	m_skinmodel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
void Enemy::Draw()
{
	m_skinmodel.Draw(
		enNormal,
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
}