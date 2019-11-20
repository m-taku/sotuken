#include "stdafx.h"
#include "Player.h"



Player::Player()
{
	m_characon.Init(
		50.0f,
		80.0f,
		m_position
	);
}


Player::~Player()
{
	delete m_state;
}

void Player::TransitionState(State m)
{
	delete m_state;
	switch (m)
	{
	case StateTownMove:
		m_state = new PlayerTownMove(this);
		break;
	case StateWate:
		m_state = new PlayerWait(this);
		break;
	default:
		break;
	}
	m_statenum = m;
}
bool Player::Start()
{
	m_skinmodel.Init(L"Assets/modelData/unityChan.cmo");
	m_skinmodel.EnableShadowCaster(true);

	CVector3 position = m_position + (m_forward * -500.0f + m_up * 100.0f);
	smGameCamera().SetPosition(position);
	smGameCamera().SetTarget(m_position + m_up * 100.0f);
	TransitionState(StateTownMove);
	DirectionLight* plight = new DirectionLight;
	plight->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	plight->SetDirection(CVector3::Down()+CVector3::Right());
	plight->SetEnableShadow(true);
	smLightManager().AddLight(plight);
	return true;
}

void Player::Update()
{
	m_state->Update();

	CVector3 move = m_position;
	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	move = m_position - move;
	cameraMovement.DefaultMove(m_position + m_up * 100.0f, move, m_forward, m_right, m_up);
	m_skinmodel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void Player::Draw()
{
	m_skinmodel.Draw(
		enNormal,
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
}
