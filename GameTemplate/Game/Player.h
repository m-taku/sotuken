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
	CVector3 m_position = {0.0f,100.0f,100.0f};		//プレイヤーのポジション
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

};

