#include "stdafx.h"
#include "Player.h"
#include"../HitManeger.h"
#include"weapon/Smallsword.h"
#include"VectorDraw.h"



Player::Player()
{
	m_characon.Init(
		80.0f,
		100.0f,
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
	switch (m)
	{
	case StateTownMove:
		delete m_state;
		m_state = new PlayerTownMove(this);
		break;
	case StateWate:
		delete m_state;
		m_state = new PlayerWait(this);
		break;
	case StateAvoid:
		if (GetPlayerNowParam().stamina >= 25&& m_isAnimtaime>=0.2f) {
			delete m_state;
			m_state = new PlayerAvoid(this);
		}
		else
		{
			m = NowState();
		}
		break;
	case StateAttack:
		delete m_state;
		m_state = new PlayerAttack(this);
		break;
	case StateAttackMode:
		delete m_state;
		m_state = new PlayerAttackMode(this);
		break;
	case Statedeath:
		delete m_state;
		m_state = new Playerdeath(this);
		break;
	case StateHit:
		delete m_state;
		m_state = new PlayerHit(this);
		break;
	case StateQuestMove:
		delete m_state;
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
	m_animClip[Hit].Load(L"Assets/animData/HardHit.tka");
	m_animClip[Hit].SetLoopFlag(false);
	m_animClip[idel].Load(L"Assets/animData/idel.tka");
	m_animClip[idel].SetLoopFlag(true);
	m_animClip[avoid].Load(L"Assets/animData/avoid.tka");
	m_animClip[avoid].SetLoopFlag(false);
	m_animClip[run].Load(L"Assets/animData/run.tka");
	m_animClip[run].SetLoopFlag(true);
	m_animClip[walk].Load(L"Assets/animData/walk.tka");
	m_animClip[walk].SetLoopFlag(true);
	m_anim.Init(m_skinmodel);
	SetAnim(m_animClip, num);
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
	GetHitObjict().Create(&m_position, 80, [&](float damage, CVector3 date) {
		HitAction(damage, date);
	}, HitObject::player);
	m_weapon = new Smallsword(this);
	m_modelpos = m_position;
	m_rig.SetBoon(m_skinmodel, L"mixamorig:Hips");
	//Playanim(run);
	move = m_position;

	//m_VectorDraw.push_back(new VectorDraw(CVector3::Zero()));
	return true;
}

void Player::Update()
{
	m_movespeed = CVector3::Zero();
	m_isAnimtaime += GetFrameDeltaTime();
	GetPlayerParam().stamina = max(0, min(GetPlayerParam().stamina + 10.0*GetFrameDeltaTime(), m_playerData.stamina));
	//if (Hp <= 0&& m_statenum != Statedeath)
	//{
	//	TransitionState(Statedeath);
	//}
	m_anim.Update(GetFrameDeltaTime() * AnimSpeed);
	m_state->Update();

	move = m_position;
	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	//if (m_anim.IsPlaying()) {
	if (!m_isAnimMove) {
		m_modelpos = m_position;
	}
	else
	{
		m_modelpos +=  m_characon.GetOffset();
	}

	move = m_position - move;
	cameraMovement.DefaultMove(m_position + m_up * 100.0f, move, m_forward, m_right, m_up);
	m_skinmodel.UpdateWorldMatrix(m_modelpos, m_rotation, m_scale);

}
void Player::PostUpdate()
{
	m_anim.Update();
}
void Player::InMovemAnim()
{
	m_isAnimMove = true;
	m_rig.StateRig(m_skinmodel.GetworldMatrix());
}
void Player::Draw()
{
	m_skinmodel.Draw(
		enNormal,
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
}
void Player::HitAction(float damage, CVector3 date)
{
	if (m_state->DamageAction(damage)) {
		if (m_playerParam.hp <= 0)
		{
			TransitionState(Statedeath);

		}
		else
		{
			TransitionState(StateHit);
			CVector3 mougaku = m_position - date;
			mougaku.y = 0.0f;
			mougaku.Normalize();
			CVector3 la = CVector3::AxisZ()*-1.0f;
			la.Normalize();
			float dotresult = la.Dot(mougaku);
			float ja = CMath::Acos(dotresult);
			if (dotresult < 0.9999f)
			{
				CVector3 jiku;
				jiku.Cross(la,mougaku);
				jiku.Normalize();
				m_rotation.SetRotation(jiku, ja);
				UpdateAxis();
			}


			/*if (jiku.y >= 0)
			{


			}
			else
			{
				m_rotation.SetRotation(CVector3::AxisY()*-1, ja);

			}*/
		}
	}
}