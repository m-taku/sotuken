#include "stdafx.h"
#include "Player.h"
#include"../HitManeger.h"
#include"weapon/Smallsword.h"



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
	delete m_weapon;
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
	case StateAvoid:
		m_state = new PlayerAvoid(this);
		break;
	case StateAttack:
		m_state = new PlayerAttack(this);
		break;
	case StateAttackMode:
		m_state = new PlayerAttackMode(this);
		break;
	case Statedeath:
		m_state = new Playerdeath(this);
		break;
	case StateQuestMove:
		m_state = new PlayerQuestMove(this);
		break;
	default:
		break;
	}
	m_statenum = m;
}
bool Player::Start()
{

	smGameCamera().Init();
	m_skinmodel.Init(L"Assets/modelData/Player.cmo");
	//m_animClip[attack].Load(L"Assets/animData/death.tka");
	//m_animClip[attack].SetLoopFlag(false); 
	m_animClip[death].Load(L"Assets/animData/death.tka");
	m_animClip[death].SetLoopFlag(false);
	m_animClip[idel].Load(L"Assets/animData/idel.tka");
	m_animClip[idel].SetLoopFlag(true); 
	m_animClip[avoid].Load(L"Assets/animData/avoid.tka");
	m_animClip[avoid].SetLoopFlag(false);
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
	plight->SetDirection(CVector3::Down() + CVector3::Right() + CVector3::Front());
	plight->ShadowEnable(true);
	smLightManager().AddLight(plight);
	GetHitObjict().Create(&m_position, 500, [&](float damage) {
		HitAction(damage);
	}, HitObject::player);
	m_weapon = new Smallsword(this);
	m_modelpos = m_position;
	m_rig.SetBoon(m_skinmodel, L"mixamorig:Hips");
	//Playanim(run);
	return true;
}

void Player::Update()
{
	//if (Hp <= 0&& m_statenum != Statedeath)
	//{
	//	TransitionState(Statedeath);
	//}
	m_state->Update();	
	CVector3 move = m_position;
	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	if (m_anim.IsPlaying()) {
		if (!m_isAnimMove) {
			m_modelpos = m_position;
		}
		else
		{
			m_modelpos += m_characon.GetOffset();
		}
	}
	move = m_position - move;
	cameraMovement.DefaultMove(m_position + m_up * 100.0f, move, m_forward, m_right, m_up);
	m_skinmodel.UpdateWorldMatrix(m_modelpos, m_rotation, m_scale);
}
void Player::PostUpdate()
{
	m_anim.Update(GetFrameDeltaTime());
}
void Player::InMovemAnim()
{
	m_isAnimMove = true;
	m_rig.StateRig();
}
void Player::Draw()
{
	m_skinmodel.Draw(
		enNormal,
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
}
void Player::HitAction(float damage)
{
	if (m_statenum != Statedeath)
	{
		m_state->DamageAction(damage);
		if (m_playerParam.hp <= 0)
		{
			TransitionState(Statedeath);

		}
	}
}