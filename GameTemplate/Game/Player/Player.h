#pragma once
#include "PlayerMovement.h"
#include"../Test_PlayerData.h"
#include "CameraMovement.h"
#include "weapon/PlayerCombo.h"

#include"PlayerStateALL.h"
#include "PlayerState.h"
#include"../Rig.h"
class HitObject;
class Player :public IGameObject
{
	friend PlayerMovement;
	friend PlayerState;
	friend PlayerCombo;
public:
/// <summary>
/// 状態のEnum
/// </summary>

	enum anim {
		walk, 
		death,
		idel,
		Hit,
		avoid,
		run,
		num
	};
	Player();
	~Player();
	bool Start();
	void Update();
	void PostUpdate();
	//void PostDraw();
	void TransitionState(State m);
	void Draw();
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	const CVector3& GetReturnPos() const
	{
		return m_returnpos;
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
	void SetReturnPos(const CVector3& pos)
	{
		m_returnpos = pos;
	}
	void Playanim(int No,bool furag = false,float time =0.2f)
	{
		m_anim.Play(No, time);
		m_modelpos = m_position;
		//m_skinmodel.UpdateWorldMatrix(m_modelpos, m_rotation, m_scale);
		
		if (furag) {
			InMovemAnim();
		}
		else
		{
			m_isAnimMove = false;
		}
	}
	void resetAnimtaime()
	{
		m_isAnimtaime = 0.0f;
	}
	bool Isfokan()
	{
		return m_anim.IsInterpolate();
	}
	bool IsPlayinganim()
	{
		return m_anim.IsPlaying();
	}
	bool IsAnimEvent(int No = 0)
	{
		return m_anim.IsEvent(No);
	}
	void SetAnim(AnimationClip animation[],int numAnimClip)
	{
		m_anim.AddAnimation(animation, numAnimClip);
	}
	void Setweapon()
	{
		m_weapon->SetAnimation(this);
	}
	PlayerCombo* Getcombo()
	{
		return m_weapon;
	}
	const Bone& GetBone(const wchar_t* name)
	{
		return *m_skinmodel.FindBone(name);
	}
	Skeleton& GetSkeleton()
	{
		return m_skinmodel.GetSkeleton();
	}
	void InMovemAnim();
	float GetAttack()
	{
		return m_playerData.attackPower;
	}
	const PalyerData& GetPlayerData()
	{
		return m_playerData;
	}
	State& NowState()
	{
		return m_statenum;
	}

	//ステータス確認
	const PalyerExecuteParam& GetPlayerNowParam()
	{
		return m_playerParam;
	}
	//ステータス変更用
	PalyerExecuteParam& GetPlayerParam()
	{
		return m_playerParam;
	}
protected:

	void HitAction(float damage,CVector3 date);
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
	DirectionLight* plight = nullptr;
	CVector3 move;
	int animNo = 0;
	CVector3 m_position = { 0.0f,100.0f,100.0f };		//プレイヤーのポジション
	CVector3 m_returnpos = { 0.0f,100.0f,100.0f };		//プレイヤーのポジション
	CVector3 m_modelpos = { 0.0f,100.0f,100.0f };		//プレイヤーのポジション
	CVector3 m_movespeed = CVector3::Zero();	//移動速度
	CVector3 m_forward = CVector3::Front();		//前方向
	CVector3 m_right = CVector3::Right();		//右方向
	CVector3 m_up = CVector3::Up();				//上方向
	CVector3 m_scale = CVector3::One();			//スケール
	CQuaternion m_rotation = CQuaternion::Identity();		//回転
	CMatrix m_mRot = CMatrix::Identity();					//回転後の前右後を取得するための行列
	bool m_isAnimMove = false;
	float m_isAnimtaime = 0.0f;

	PlayerCombo* m_weapon = nullptr;
	Rig m_rig;
	State m_statenum = StateTownMove;
	Animation m_anim;
	PalyerData m_playerData;
	PalyerExecuteParam m_playerParam;
	AnimationClip m_animClip[num];
	PlayerState* m_state = nullptr;
	SkinModel m_skinmodel;		//スキンモデル
	CharacterController m_characon;		//キャラコン
	CameraMovement cameraMovement;
	//dededede
	//Carving hagihagi;
	float AnimSpeed = 1.0f;
//	std::vector<VectorDraw*> m_VectorDraw;

};

