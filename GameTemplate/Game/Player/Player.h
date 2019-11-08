#pragma once
#include "PlayerMovement.h"
#include "CameraMovement.h"

class Player :public IGameObject
{
	friend PlayerMovement;
public:
/// <summary>
/// 状態のEnum
/// </summary>
	enum State {
		State_Attack,		//攻撃中
		State_Special,		//特殊攻撃中
		State_Move,			//移動中
		State_Guard,		//防御中
		State_Hit,			//ヒット中
		State_did			//死亡中
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
	CVector3 m_position = { 0.0f,100.0f,100.0f };		//プレイヤーのポジション
	CVector3 m_movespeed = CVector3::Zero();	//移動速度
	CVector3 m_forward = CVector3::Front();		//前方向
	CVector3 m_right = CVector3::Right();		//右方向
	CVector3 m_up = CVector3::Up();				//上方向
	CVector3 m_scale = CVector3::One();			//スケール

	CQuaternion m_rotation = CQuaternion::Identity();		//回転
	CMatrix m_mRot = CMatrix::Identity();					//回転後の前右後を取得するための行列

	SkinModel m_skinmodel;		//スキンモデル
	CharacterController m_characon;		//キャラコン
	PlayerMovement Movement;
	CameraMovement cameraMovement;
};

