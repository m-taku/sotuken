#pragma once
#include "character/CharacterController.h"
class TEstNPC : public IGameObject
{
public:
	TEstNPC();
	~TEstNPC();
	bool State();
	void Update();
	void Draw();
private:
	int No = 0;
	CharacterController m_collider;						//キャラクターコントローラー
	CVector3 posm = CVector3::Zero();
	CVector3 pos = { 0.0f,0.0f,0.0f };
	SkinModel m_model;	
	SkinModel m_bikkuri;
};

