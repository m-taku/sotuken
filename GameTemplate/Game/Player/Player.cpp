#include "stdafx.h"
#include "Player.h"



Player::Player()
{
	Movement.SetPlayer(this);

}


Player::~Player()
{
	delete &Movement;
	delete &cameraMovement;
}
void Player::TransitionState(State state)
{
	//delete m_State;
	//switch (state)
	//{
	//case State_Attack:
	//	m_State = new Player_Attack(this);
	//	break;
	//case State_Special:
	//	m_State = new Player_Special(this);
	//	break;
	//case State_Move:
	//	m_State = new Player_Move(this);
	//	break;
	//case State_Guard:
	//	m_State = new Player_Guard(this);
	//	break;
	//case State_Hit:
	//	m_State = new Player_Hit(this);
	//	break;
	//case State_did:
	//	m_State = new Player_Die(this);
	//	break;
	//default:
	//	break;
	//}
	//m_state = state;
	//m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
	//m_animation.Update(GetTime().GetFrameTime());
}
bool Player::Start()
{
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
	Movement.DefaultMove();

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
