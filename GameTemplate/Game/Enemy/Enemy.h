#pragma once
#include"EnemyStatus.h"
#include"EnemyState.h"
#include"EnemyStateList.h"
#include"enemyname/monster.h"
#include"Carving.h"
class HitObject;
class VectorDraw;
class Enemy : public IGameObject
{
public:
	friend EnemyStateAttack;
	friend EnemyStateDead;
	friend EnemyStateLoitering;
	friend monster;
	Enemy();
	~Enemy();	
/// <summary>
/// ��Ԃ�Enum
/// </summary>
	enum StateEnemy {
		StateLoitering,			//�ړ���
		StateAttack,
		StateDead
	};
	bool Start();
	void Update();
	void TransitionState(StateEnemy m);
	void Draw();
	void PostUpdate();
	void OnDestroy();
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
	bool IsPlayinganim()
	{
		return m_anim.IsPlaying();
	}
	void Playanim(int No)
	{
		m_anim.Play(No, 0.2f);
	}
	void SetAnim(AnimationClip animation[], int numAnimClip)
	{
		m_anim.AddAnimation(animation, numAnimClip);
	}
	bool IsAnimEvent(int No = 1)
	{
		return m_anim.IsEvent(No);
	}
	StateEnemy GetState()
	{
		return m_statenum;
	}
	void SetAnimdate(SkinModel& ka)
	{
		m_anim.Init(ka);
	}
	void HitAction(float damage);
protected:
	monster* m_monster = nullptr;
	EnemyStatus* m_status = nullptr;

private:
	CVector3 m_position = { 0.0f,100.0f,100.0f };		//�v���C���[�̃|�W�V����
	CVector3 m_movespeed = CVector3::Zero();	//�ړ����x
	CVector3 m_forward = CVector3::Front();		//�O����
	CVector3 m_right = CVector3::Right();		//�E����
	CVector3 m_up = CVector3::Up();				//�����
	CVector3 m_scale = CVector3::One();			//�X�P�[��

	Animation m_anim;
	CQuaternion m_rotation = CQuaternion::Identity();		//��]
	CMatrix m_mRot = CMatrix::Identity();					//��]��̑O�E����擾���邽�߂̍s��
	StateEnemy m_statenum = StateLoitering;
	EnemyState* m_state = nullptr;
	CharacterController m_characon;		//�L�����R��
	Player* m_player = nullptr;
	const HitObject* m_HitObject = nullptr;
	float HP = 10.0f;
	//�f�o�b�N�p�ϐ�
	Carving hagihagi;
	float debugtaim = 0.0f;
	std::vector<VectorDraw*> m_VectorDraw;
};

