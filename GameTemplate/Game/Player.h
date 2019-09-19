#pragma once
#include "PlayerMovement.h"
class Player:public IGameObject
{
	friend PlayerMovement;
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Draw();
private:
	CVector3 m_position = {0.0f,100.0f,100.0f};		//�v���C���[�̃|�W�V����
	CVector3 m_movespeed = CVector3::Zero();	//�ړ����x
	CVector3 m_forward = CVector3::Front();		//�O����
	CVector3 m_right = CVector3::Right();		//�E����
	CVector3 m_up = CVector3::Up();				//�����
	CVector3 m_scale = CVector3::One();			//�X�P�[��

	CQuaternion m_rotation = CQuaternion::Identity();		//��]
	CMatrix m_mRot = CMatrix::Identity();					//��]��̑O�E����擾���邽�߂̍s��

	SkinModel m_skinmodel;		//�X�L�����f��
	CharacterController m_characon;		//�L�����R��
	PlayerMovement Movement;

};

