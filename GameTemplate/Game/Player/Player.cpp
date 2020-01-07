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
	case Statedeath:
		m_state = new Playerdeath(this);
		break;
	default:
		break;
	}
	m_statenum = m;
}
bool Player::Start()
{
	smGameCamera().Init();
	m_skinmodel.Init(L"Assets/modelData/unityChan.cmo");
	m_animClip[attack].Load(L"Assets/animData/death.tka");
	m_animClip[attack].SetLoopFlag(false); 
	m_animClip[idel].Load(L"Assets/animData/idel.tka");
	m_animClip[idel].SetLoopFlag(true);
	m_animClip[run].Load(L"Assets/animData/run.tka");
	m_animClip[run].SetLoopFlag(true);
	m_animClip[walk].Load(L"Assets/animData/walk.tka");
	m_animClip[walk].SetLoopFlag(true);
	m_anim.Init(m_skinmodel, m_animClip, num);
	m_skinmodel.EnableShadowCaster(true);
	CVector3 position = m_position + (m_forward * -500.0f + m_up * 100.0f);
	smGameCamera().SetPosition(position);
	smGameCamera().SetTarget(m_position + m_up * 100.0f);
	TransitionState(StateTownMove);
	DirectionLight* plight = new DirectionLight;
	float c = 1.0f;
	plight->SetColor({ c,c,c,1.0f });
	plight->SetDirection(CVector3::Down()+CVector3::Right());
	plight->ShadowEnable(true);
	smLightManager().AddLight(plight);
	return true;
}

void Player::Update()
{
	if (Hp <= 0&& m_statenum != Statedeath)
	{
		TransitionState(Statedeath);
	}
	m_state->Update();

	CVector3 move = m_position;
	//m_movespeed.z += 10.0f;
	//m_movespeed.y -= 0.001f;
	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	if (m_movespeed.Length() != 0.0f&&m_statenum != Statedeath)
	{
		Playanim(run);
	}
	else if(m_statenum != Statedeath)
	{
		Playanim(idel);
	}
	move = m_position - move;
	cameraMovement.DefaultMove(m_position + m_up * 100.0f, move, m_forward, m_right, m_up);
	m_skinmodel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}
void  Player::PostDraw()
{
	m_anim.Update(GetFrameDeltaTime());

}

void Player::Draw()
{
	m_skinmodel.Draw(
		enNormal,
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
}
