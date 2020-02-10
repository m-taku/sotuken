#include "stdafx.h"
#include "PlayerMovement.h"
#include "Player.h"

PlayerMovement::PlayerMovement()
{
}


PlayerMovement::~PlayerMovement()
{
	m_player->m_movespeed = CVector3::Zero();
}

void PlayerMovement::TounMove()
{
	m_player->UpdateAxis();
	m_player->m_movespeed = CVector3::Zero();
	float padinput_LX = g_pad[0].GetLStickXF()*500.0f;
	float padinput_LY = g_pad[0].GetLStickYF()*500.0f;
	m_addGravityTime += 1.0f*GetFrameDeltaTime();
	m_fallSpeed = (GRAVITY_PARAM*pow(m_addGravityTime, 2.0f)) * 0.5f;
	m_player->m_movespeed.y -= m_fallSpeed;
	if (m_player->m_characon.IsOnGround())
	{
		m_addGravityTime = 0.0f;
		m_fallSpeed = 0.0f;
		m_player->m_movespeed.y = 0.0f;

	}
	//m_player->m_movespeed.y *= 0.5f;
	CVector3 camera_z = smGameCamera().GetCameraFoward();
	camera_z.y = 0.0f;
	camera_z.Normalize();
	CVector3 camera_x = smGameCamera().GetCameraRight();
	camera_x.y = 0.0f;
	camera_x.Normalize();

	CVector3 camera_XZ = camera_z * padinput_LY + camera_x * padinput_LX;
	m_player->m_movespeed.x = camera_XZ.x;
	m_player->m_movespeed.z = camera_XZ.z;

	//プレイヤーの回転処理
	CVector3 vec = camera_XZ;

	if (vec.Length() > 0.0f)
	{
		vec.Normalize();
		float angle = CMath::Acos(m_player->m_forward.Dot(vec));
		if (angle > 0.0f);
		{
			CVector3 axis;
			axis.Cross(m_player->m_forward, vec);
			axis.Normalize();
			CQuaternion qRot;

			qRot.SetRotation(axis, angle);
			m_player->m_rotation.Multiply(qRot);
			m_player->UpdateAxis();
		}
	}
}
//void PlayerMovement::QuestMove()
//{
//	m_player->UpdateAxis();
//	m_player->m_movespeed = CVector3::Zero();
//	float padinput_LX = g_pad[0].GetLStickXF()*500.0f;
//	float padinput_LY = g_pad[0].GetLStickYF()*500.0f;
//	m_addGravityTime += 1.0f*GetFrameDeltaTime();
//	m_fallSpeed = (GRAVITY_PARAM*pow(m_addGravityTime, 2.0f)) * 0.5f;
//	m_player->m_movespeed.y -= m_fallSpeed;
//	if (m_player->m_characon.IsOnGround())
//	{
//		m_addGravityTime = 0.0f;
//		m_fallSpeed = 0.0f;
//		m_player->m_movespeed.y = 0.0f;
//
//	}
//	//m_player->m_movespeed.y *= 0.5f;
//	CVector3 camera_z = smGameCamera().GetCameraFoward();
//	camera_z.y = 0.0f;
//	camera_z.Normalize();
//	CVector3 camera_x = smGameCamera().GetCameraRight();
//	camera_x.y = 0.0f;
//	camera_x.Normalize();
//
//	CVector3 camera_XZ = camera_z * padinput_LY + camera_x * padinput_LX;
//	m_player->m_movespeed.x = camera_XZ.x;
//	m_player->m_movespeed.z = camera_XZ.z;
//
//	//プレイヤーの回転処理
//	CVector3 vec = camera_XZ;
//
//	if (vec.Length() > 0.0f)
//	{
//		vec.Normalize();
//		float angle = CMath::Acos(m_player->m_forward.Dot(vec));
//		if (angle > 0.0f);
//		{
//			CVector3 axis;
//			axis.Cross(m_player->m_forward, vec);
//			axis.Normalize();
//			CQuaternion qRot;
//
//			qRot.SetRotation(axis, angle);
//			m_player->m_rotation.Multiply(qRot);
//			m_player->UpdateAxis();
//		}
//	}
//}
void PlayerMovement::QuestMove()
{
	m_player->UpdateAxis();
	m_player->m_movespeed = CVector3::Zero();
	float padinput_LX = g_pad[0].GetLStickXF()*500.0f;
	float padinput_LY = g_pad[0].GetLStickYF()*500.0f;
	m_addGravityTime += 1.0f*GetFrameDeltaTime();
	m_fallSpeed = (GRAVITY_PARAM*pow(m_addGravityTime, 2.0f)) * 0.5f;
	m_player->m_movespeed.y -= m_fallSpeed;
	if (m_player->m_characon.IsOnGround())
	{
		m_addGravityTime = 0.0f;
		m_fallSpeed = 0.0f;
		m_player->m_movespeed.y = 0.0f;

	}
	//m_player->m_movespeed.y *= 0.5f;
	CVector3 camera_z = smGameCamera().GetCameraFoward();
	camera_z.y = 0.0f;
	camera_z.Normalize();
	CVector3 camera_x = smGameCamera().GetCameraRight();
	camera_x.y = 0.0f;
	camera_x.Normalize();

	CVector3 camera_XZ = camera_z * padinput_LY + camera_x * padinput_LX;
	if (g_pad[0].IsPress(enButtonRB1)&& m_player->m_playerParam.stamina > 0)
	{
		camera_XZ *= 1.3f;
		m_player->m_playerParam.stamina = max(m_player->m_playerParam.stamina - 20.0*GetFrameDeltaTime(), 0.0f);
	}
	m_player->m_movespeed.x = camera_XZ.x;
	m_player->m_movespeed.z = camera_XZ.z;

	//プレイヤーの回転処理
	CVector3 vec = camera_XZ;

	if (vec.Length() > 0.0f)
	{
		vec.Normalize();
		float angle = CMath::Acos(m_player->m_forward.Dot(vec));
		if (angle > 0.0f);
		{
			CVector3 axis;
			axis.Cross(m_player->m_forward, vec);
			axis.Normalize();
			CQuaternion qRot;

			qRot.SetRotation(axis, angle);
			m_player->m_rotation.Multiply(qRot);
			m_player->UpdateAxis();
		}
	}
}
void PlayerMovement::QuestWeaponMove()
{
	m_player->UpdateAxis();
	m_player->m_movespeed = CVector3::Zero();
	float padinput_LX = g_pad[0].GetLStickXF()*500.0f;
	float padinput_LY = g_pad[0].GetLStickYF()*500.0f;
	m_addGravityTime += 1.0f*GetFrameDeltaTime();
	m_fallSpeed = (GRAVITY_PARAM*pow(m_addGravityTime, 2.0f)) * 0.5f;
	m_player->m_movespeed.y -= m_fallSpeed;
	if (m_player->m_characon.IsOnGround())
	{
		m_addGravityTime = 0.0f;
		m_fallSpeed = 0.0f;
		m_player->m_movespeed.y = 0.0f;

	}
	//m_player->m_movespeed.y *= 0.5f;
	CVector3 camera_z = smGameCamera().GetCameraFoward();
	camera_z.y = 0.0f;
	camera_z.Normalize();
	CVector3 camera_x = smGameCamera().GetCameraRight();
	camera_x.y = 0.0f;
	camera_x.Normalize();

	CVector3 camera_XZ = camera_z * padinput_LY + camera_x * padinput_LX;
	camera_XZ /= m_player->m_weapon->GetWeight();
	m_player->m_movespeed.x = camera_XZ.x;
	m_player->m_movespeed.z = camera_XZ.z;

	
	//プレイヤーの回転処理
	CVector3 vec = camera_XZ;

	if (vec.Length() > 0.0f)
	{
		vec.Normalize();
		float angle = CMath::Acos(m_player->m_forward.Dot(vec));
		if (angle > 0.0f);
		{
			CVector3 axis;
			axis.Cross(m_player->m_forward, vec);
			axis.Normalize();
			CQuaternion qRot;

			qRot.SetRotation(axis, angle);
			m_player->m_rotation.Multiply(qRot);
			m_player->UpdateAxis();
		}
	}
}
void PlayerMovement::AttackMove()
{
	m_player->m_movespeed = CVector3::Zero();	

	if (m_player->m_isAnimMove)
	{
		m_player->m_movespeed = m_player->m_rig.Updete();
		m_player->m_movespeed /= GetFrameDeltaTime();
	}	

	m_addGravityTime += 1.0f*GetFrameDeltaTime();
	m_fallSpeed = (GRAVITY_PARAM*pow(m_addGravityTime, 2.0f)) * 0.5f;
	//m_player->m_movespeed.y -= m_fallSpeed;
	if (m_player->m_characon.IsOnGround())
	{
		m_addGravityTime = 0.0f;
		m_fallSpeed = 0.0f;
		m_player->m_movespeed.y = 0.0f;
	}
}