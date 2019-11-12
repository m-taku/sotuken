#include "stdafx.h"
#include "PlayerMovement.h"
#include "Player.h"

PlayerMovement::PlayerMovement()
{
}


PlayerMovement::~PlayerMovement()
{
}

void PlayerMovement::DefaultMove()
{
	m_player->m_movespeed = CVector3::Zero();
	float padinput_LX = g_pad[0].GetLStickXF();
	float padinput_LY = g_pad[0].GetLStickYF();
	m_addGravityTime += 1.0f*GetFrameDeltaTime();
	m_fallSpeed += (GRAVITY_PARAM*pow(m_addGravityTime, 2.0f)) / 2.0f;
	m_player->m_movespeed.y -= m_fallSpeed;
	if (m_player->m_characon.IsOnGround())
	{
		IsJump = false;
		m_addGravityTime = 0.0f;
		m_fallSpeed = 0.0f;
		m_player->m_movespeed.y = 0.0f;
		if (g_pad[0].IsTrigger(enButtonA)) IsJump = true;

	}
	if (IsJump) m_player->m_movespeed.y += m_jumpSpeed;

	CVector3 camera_z = smGameCamera().GetCameraFoward();
	camera_z.y = 0.0f;
	camera_z.Normalize();
	CVector3 camera_x = smGameCamera().GetCameraRight();
	camera_x.y = 0.0f;
	camera_x.Normalize();

	CVector3 camera_XZ = camera_z * padinput_LY + camera_x * padinput_LX;
	camera_XZ.y = 0.0f;
	camera_XZ.Normalize();
	camera_XZ *= 500.0f;
	m_player->m_movespeed.x = camera_XZ.x;
	m_player->m_movespeed.z = camera_XZ.z;
}
