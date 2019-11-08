#include "stdafx.h"
#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
	delete &cameraMovement;
}
void Player::TransitionState(State state)
{
	delete m_state;
	switch (state)
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
	m_statenum = state;
	//m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
	//m_animation.Update(GetTime().GetFrameTime());
}
bool Player::Start()
{
	TransitionState(StateTownMove);
	m_skinmodel.Init(L"Assets/modelData/unityChan.cmo");
	m_position.y += 500.0f;
	m_position.x -= 300.0f;
	m_characon.Init(
		50.0f,
		80.0f,
		m_position
	);
	CVector3 position = m_position + (m_forward * -100.0f + m_up * 100.0f);
	smGameCamera().SetPosition(position);
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
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
}
