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
	enum anim {
		attack,
		num
	};
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
		m_anim.Play(No, 0.0f);
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
	void HitAction(float damage);
protected:
	monster* m_monster = nullptr;
	EnemyStatus* m_status = nullptr;

private:
	Animation m_anim;
	AnimationClip m_animClip[num];
	CVector3 m_position = { 0.0f,100.0f,100.0f };		//プレイヤーのポジション
	CVector3 m_movespeed = CVector3::Zero();	//移動速度
	CVector3 m_forward = CVector3::Front();		//前方向
	CVector3 m_right = CVector3::Right();		//右方向
	CVector3 m_up = CVector3::Up();				//上方向
	CVector3 m_scale = CVector3::One();			//スケール

	CQuaternion m_rotation = CQuaternion::Identity();		//回転
	CMatrix m_mRot = CMatrix::Identity();					//回転後の前右後を取得するための行列
	StateEnemy m_statenum = StateLoitering;
	EnemyState* m_state = nullptr;
	SkinModel m_skinmodel;		//スキンモデル
	CharacterController m_characon;		//キャラコン
	Player* m_player = nullptr;
	const HitObject* m_HitObject = nullptr;
	float HP = 10.0f;
	//デバック用変数
	Carving hagihagi;
	float debugtaim = 0.0f;
	std::vector<VectorDraw*> m_VectorDraw;
};

