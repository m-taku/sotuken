#include "stdafx.h"
#include "Enemy.h"
#include "EnemyStateList.h"
#include"VectorDraw.h"
#include"Route search/Path.h"
#include"../HitManeger.h"
#include"Player.h"
Enemy::Enemy()
{
	m_characon.Init(
		200.0f,
		100.0f,
		m_position
	);
}


Enemy::~Enemy()
{
	delete m_monster;
	delete m_state;
}
void Enemy::OnDestroy()
{
	GetHitObjict().Deleteobjict(m_HitObject);
}
void Enemy::TransitionState(StateEnemy m)
{
	delete m_state;
	switch (m)
	{
	case StateLoitering:
		m_state = new EnemyStateLoitering(this, m_player);
		break;
	case StateDead:
		m_state = new EnemyStateDead(this, m_player);
		break;
	case StateChase:
		m_state = new EnemyStateChase(this, m_player);
		break;
	case StateAttack:
		m_state = new EnemyStateAttack(this,m_player);
	default:
		break;
	}
	m_statenum = m;
}
bool Enemy::Start()
{
	m_Path;
	m_monster->Init();
	m_player = FindGO<Player>("player");
	m_HitObject = GetHitObjict().Create(&m_position, 500, [&](float damage, CVector3 date) {
		HitAction(damage);
	}, HitObject::enemy);
	m_monster->GetSkinModel()->EnableShadowCaster(true);
	TransitionState(m_statenum);
	return true;
}
void Enemy::Update()
{
	SetMovespeed(CVector3::Zero());
	m_anim.Update(GetFrameDeltaTime());
	m_state->Update();
	m_addGravityTime += 1.0f*GetFrameDeltaTime();
	m_fallSpeed = (GRAVITY_PARAM*pow(m_addGravityTime, 2.0f)) * 0.5f;
	m_movespeed.y -= m_fallSpeed;
	if (m_characon.IsOnGround())
	{
		m_addGravityTime = 0.0f;
		m_fallSpeed = 0.0f;
		m_movespeed.y = 0.0f;
	}
	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	UpdateAxis();
	if (!m_furag)
	{
		m_modelpos = m_position;
	}
	else
	{
		m_modelpos += m_characon.GetOffset();
	}
	m_monster->GetSkinModel()->UpdateWorldMatrix(m_modelpos, m_rotation, CVector3::One());
}
void Enemy::Draw()
{
	m_monster->GetSkinModel()->Draw(enNormal,
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix());
	m_state->Draw();
}
void Enemy::PostUpdate()
{
	m_anim.Update();
}
void Enemy::HitAction(float damage)
{
	HP -= damage;
	if (HP <= 0&&m_statenum != StateDead) {
		TransitionState(StateDead);
	}
}