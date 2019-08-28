#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/unityChan.cmo");
}


Player::~Player()
{
}

void Player::Update()
{
	static CVector3 pos = CVector3::Zero();
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(pos, CQuaternion::Identity(), CVector3::One());
}
void Player::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}