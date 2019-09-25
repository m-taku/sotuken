#include "stdafx.h"
#include "Player.h"



Player::Player()
{
	Movement.SetPlayer(this);
}


Player::~Player()
{
	delete &Movement;
}

bool Player::Start()
{
	m_skinmodel.Init(L"Assets/modelData/unityChan.cmo");
	m_characon.Init(
		50.0f,
		80.0f,
		m_position
	);

	return true;
}

void Player::Update()
{
	Movement.DefaultMove();
	

	m_position = m_characon.Execute(GetFrameDeltaTime(), m_movespeed);
	smGameCamera().SetTarget(m_position);
	m_skinmodel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void Player::Draw()
{
	m_skinmodel.Draw(
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
}
