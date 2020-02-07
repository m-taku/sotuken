#include "stdafx.h"
#include "Enemy.h"
#include "EnemyStateList.h"
#include"VectorDraw.h"
#include"../HitManeger.h"
#include"Player.h"
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
	delete m_monster;
	delete m_state;
	for (auto na : m_VectorDraw)
	{
		delete na;
	}
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
	case StateAttack:
		m_state = new EnemyStateAttack(this,m_player);
	default:
		break;
	}
	m_statenum = m;
}
bool Enemy::Start()
{
	m_monster->Init();
	m_player = FindGO<Player>("player");
	m_HitObject = GetHitObjict().Create(&m_position, 500, [&](float damage, CVector3 date) {
		HitAction(damage);
	}, HitObject::enemy);
	TransitionState(m_statenum);
	return true;
}
void Enemy::Update()
{
	m_state->Update();
	m_movespeed.y -= 9.8f;
	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	m_monster->GetSkinModel()->UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
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
	m_anim.Update(GetFrameDeltaTime());
	m_anim.Update();
}
void Enemy::HitAction(float damage)
{
	HP -= damage;
	if (HP <= 0&&m_statenum != StateDead) {
		TransitionState(StateDead);
	}
}