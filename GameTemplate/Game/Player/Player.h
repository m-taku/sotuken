#pragma once
#include "PlayerMovement.h"
#include "CameraMovement.h"

class Player :public IGameObject
{
	friend PlayerMovement;
public:
/// <summary>
/// ��Ԃ�Enum
/// </summary>
	enum State {
		State_Attack,		//�U����
		State_Special,		//����U����
		State_Move,			//�ړ���
		State_Guard,		//�h�䒆
		State_Hit,			//�q�b�g��
		State_did			//���S��
	};
	Player();
	~Player();
	bool Start();
	void Update();
	void TransitionState(State );
	void Draw();
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	const CVector3& GetForward() const
	{
		return m_forward;
	}
	const CVector3& GetUp() const
	{
		return m_up;
	}
	const CVector3& GetRight() const
	{
		return m_right;
	}
	const CVector3& GetMoveSpeed() const
	{
		return m_movespeed;
	}
	const CQuaternion& GetRotation() const
	{
		return m_rotation;
	}
private:
	CVector3 m_position = { 0.0f,100.0f,100.0f };		//�v���C���[�̃|�W�V����
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
	CameraMovement cameraMovement;
};

