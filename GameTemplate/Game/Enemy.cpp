#include "stdafx.h"
#include "Enemy.h"
#include"EnemyStateLoitering.h"
#include"EnemyStateDead.h"

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
void Enemy::TransitionState(StateEnemy m)
{
	delete m_state;
	switch (m)
	{
	case StateLoitering:
		m_state = new EnemyStateLoitering(this);
		break;
	case StateDead:
		m_state = new EnemyStateDead(this);
		break;
	default:
		break;
	}
	m_statenum = m;
}
bool Enemy::Start()
{
	m_skinmodel.Init(L"Assets/modelData/Dragon_1.cmo");
	m_skinmodel.EnableShadowCaster(true);
	TransitionState(m_statenum);
	return true;
}
void Enemy::Update()
{
	debugtaim += GetFrameDeltaTime();
	if (debugtaim >= 30.0f)
	{
		debugtaim = -FLT_MAX;
		TransitionState(StateDead);
	}
	m_state->Update();
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