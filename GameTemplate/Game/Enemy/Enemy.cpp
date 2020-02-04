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
	case StateAttack:
		m_state = new EnemyStateAttack(this);
	default:
		break;
	}
	m_statenum = m;
}
bool Enemy::Start()
{
	m_skinmodel.Init(L"Assets/modelData/Dragon_2.cmo");
	m_animClip[attack].Load(L"Assets/animData/dragonattack.tka");
	m_animClip[attack].SetLoopFlag(true);
	m_anim.Init(m_skinmodel, m_animClip, num);
	m_skinmodel.EnableShadowCaster(true);
	m_anim.Play(attack); 
	for (int i = 0; i < m_skinmodel.GetSkeleton().GetNumBones(); i++)
	{
		m_VectorDraw.push_back(new VectorDraw(CVector3::Zero()));
	}
	GetHitObjict().Create(&m_position, 500, [&](float damage) {
		HitAction(damage);
	}, HitObject::enemy);
	TransitionState(m_statenum);
	return true;
}
void Enemy::Update()
{
	for (int i = 0; i < m_VectorDraw.size(); i++) {
		auto n = m_skinmodel.GetSkeleton().GetBone(i);
		auto mamma = n->GetWorldMatrix();
		CVector3 pos = CVector3::Zero();
		pos.x = mamma.m[3][0];
		pos.y = mamma.m[3][1];
		pos.z = mamma.m[3][2];
		CVector3 m_papa;
		m_papa.x = mamma.m[2][0];
		m_papa.y = mamma.m[2][1];
		m_papa.z = mamma.m[2][2];
		m_VectorDraw[i]->Update(pos, m_papa, 50.0f);
	}
	m_state->Update();
	//m_movespeed.y -= 9.8f;
	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	m_skinmodel.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());
}
void Enemy::Draw()
{
	m_skinmodel.Draw(
		enNormal,
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
	m_state->Draw();
}
void Enemy::PostUpdate()
{
	auto jra = m_skinmodel.FindBone(L"R Elbow");
	jra->GetNo();
	m_VectorDraw[jra->GetNo()]->Draw();
	/*for (const auto& Vector : m_VectorDraw) {
		Vector->Draw();
	}*/
	if (g_pad[0].IsPress(enButtonDown))
	{
		m_anim.Update(GetFrameDeltaTime());
	}
}
void Enemy::HitAction(float damage)
{
	HP -= damage;
	if (HP <= 0&&m_statenum != StateDead) {
		TransitionState(StateDead);
	}
}