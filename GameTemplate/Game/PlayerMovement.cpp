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
	padinput_LX = g_pad[0].GetLStickXF();
	padinput_LY = g_pad[0].GetLStickYF();

	if (!m_player->m_characon.IsOnGround())
	{
		m_gravity += 980.0f*GetFrameDeltaTime();
		m_player->m_movespeed.y -= m_gravity;
	}
	else
	{
		m_gravity = 0.0f;
		m_player->m_movespeed.y = 0.0f;
	}
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
	m_player->m_movespeed += camera_XZ;

	CVector3 toMainCameraTarget = m_player->m_position - smGameCamera().GetCameraPosition();
	CVector3 toSubCameraTarget = (m_player->m_position + m_player->m_movespeed) - smGameCamera().GetCameraPosition();

}

void PlayerMovement::CameraMove()
{

}
