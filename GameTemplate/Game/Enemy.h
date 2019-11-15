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
/// 状態のEnum
/// </summary>
	enum StateEnemy {
		StateLoitering,			//移動中
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



	//デバック用変数
	float debugtaim = 0.0f;
	
};

