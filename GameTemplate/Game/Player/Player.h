#pragma once
#include "PlayerMovement.h"
#include "CameraMovement.h"
#include"PlayerStateALL.h"
#include "PlayerState.h"

class Player :public IGameObject
{
	friend PlayerMovement;
public:
/// <summary>
/// ��Ԃ�Enum
/// </summary>
	enum State {
		StateTownMove,			//�ړ���
		StateWate,
		Statedeath
	};
	enum anim {
		attack,
		idel,
		walk,
		run,
		num
	};
	Player();
	~Player();
	bool Start();
	void Update();
	void PostDraw();
	void TransitionState(State m);
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
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	void SetMovespeed(const CVector3& move)
	{
		m_movespeed = move;
	}
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
		m_characon.SetPosition(pos);
	}
	void Playanim(anim No)
	{
		m_anim.Play(No,0.2f);
	}
	int Hp = 1;
private:
	void UpdateAxis()
	{
		CMatrix mRot;
		mRot.MakeRotationFromQuaternion(m_rotation);
		
		m_right = CVector3(mRot.m[0][0], mRot.m[0][1], mRot.m[0][2]);
		m_up = CVector3(mRot.m[1][0], mRot.m[1][1], mRot.m[1][2]);
		m_forward = CVector3(mRot.m[2][0], mRot.m[2][1], mRot.m[2][2]);
		m_right.Normalize();
		m_up.Normalize();
		m_forward.Normalize();
	}
	CVector3 m_position = { 0.0f,100.0f,100.0f };		//�v���C���[�̃|�W�V����
	CVector3 m_movespeed = CVector3::Zero();	//�ړ����x
	CVector3 m_forward = CVector3::Front();		//�O����
	CVector3 m_right = CVector3::Right();		//�E����
	CVector3 m_up = CVector3::Up();				//�����
	CVector3 m_scale = CVector3::One();			//�X�P�[��
	CQuaternion m_rotation = CQuaternion::Identity();		//��]
	CMatrix m_mRot = CMatrix::Identity();					//��]��̑O�E����擾���邽�߂̍s��
	State m_statenum = StateTownMove;
	Animation m_anim;
	AnimationClip m_animClip[num];
	PlayerState* m_state = nullptr;
	SkinModel m_skinmodel;		//�X�L�����f��
	CharacterController m_characon;		//�L�����R��
	CameraMovement cameraMovement;
};

