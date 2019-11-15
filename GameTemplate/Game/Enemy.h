#pragma once
#include"EnemyStatus.h"
#include"EnemyState.h"
class Enemy : public IGameObject
{
	friend EnemyState;
public:
	Enemy();
	~Enemy();
/// <summary>
/// ��Ԃ�Enum
/// </summary>
	enum StateEnemy {
		StateLoitering,			//�ړ���
		StateDead
	};
	bool Start();
	void Update();
	void TransitionState(StateEnemy m);
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
	}void SetMovespeed(const CVector3& move)
	{
		m_movespeed = move;
	}
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
		m_characon.SetPosition(pos);
	}
protected:
	EnemyStatus* m_status = nullptr;
private:
	CVector3 m_position = { 0.0f,100.0f,100.0f };		//�v���C���[�̃|�W�V����
	CVector3 m_movespeed = CVector3::Zero();	//�ړ����x
	CVector3 m_forward = CVector3::Front();		//�O����
	CVector3 m_right = CVector3::Right();		//�E����
	CVector3 m_up = CVector3::Up();				//�����
	CVector3 m_scale = CVector3::One();			//�X�P�[��

	CQuaternion m_rotation = CQuaternion::Identity();		//��]
	CMatrix m_mRot = CMatrix::Identity();					//��]��̑O�E����擾���邽�߂̍s��
	StateEnemy m_statenum = StateLoitering;
	EnemyState* m_state = nullptr;
	SkinModel m_skinmodel;		//�X�L�����f��
	CharacterController m_characon;		//�L�����R��



	//�f�o�b�N�p�ϐ�
	float debugtaim = 0.0f;
	
};

