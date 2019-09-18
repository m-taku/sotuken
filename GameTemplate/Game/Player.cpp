#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}

bool Player::Start()
{
	m_skinmodel.Init(L"Assets/modelData/unityChan.cmo");
	return true;
}

void Player::Update()
{
	m_skinmodel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void Player::Draw()
{
	m_skinmodel.Draw(
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
}
