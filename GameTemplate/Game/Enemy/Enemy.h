#pragma once
#include"EnemyStatus.h"
#include"EnemyState.h"
#include"EnemyStateList.h"
#include"enemyname/monster.h"
#include"Carving.h"
class HitObject;
class Path;
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
/// 状態のEnum
/// </summary>
	enum StateEnemy {
		StateLoitering,			//移動中
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
	void Playanim(int No,bool furag = false)
	{
		m_anim.Play(No, 0.2f);
		m_furag = furag;
		if (!m_furag)
		{
			m_modelpos = m_position;
		}

		//m_modelpos = m_position;
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
	Path* CopyPath()
	{
		return &m_Path;
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
	CVector3 m_position = { 0.0f,100.0f,100.0f };		//プレイヤーのポジション
	CVector3 m_modelpos = { 0.0f,100.0f,100.0f };		//プレイヤーのポジション
	CVector3 m_movespeed = CVector3::Zero();	//移動速度
	CVector3 m_forward = CVector3::Front();		//前方向
	CVector3 m_right = CVector3::Right();		//右方向
	CVector3 m_up = CVector3::Up();				//上方向
	CVector3 m_scale = CVector3::One();			//スケール


	Path m_Path;
	Animation m_anim;
	CQuaternion m_rotation = CQuaternion::Identity();		//回転
	CMatrix m_mRot = CMatrix::Identity();					//回転後の前右後を取得するための行列
	StateEnemy m_statenum = StateLoitering;
	EnemyState* m_state = nullptr;
	CharacterController m_characon;		//キャラコン
	Player* m_player = nullptr;
	const HitObject* m_HitObject = nullptr;
	float HP = 10.0f;
	bool m_furag = false;
	//デバック用変数
	Carving hagihagi;
	float debugtaim = 0.0f;
};

