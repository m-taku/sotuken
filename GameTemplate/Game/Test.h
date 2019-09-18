#pragma once
#include "character/CharacterController.h"
class Test : public IGameObject
{
public:
	Test();
	~Test();
	bool Start();
	void Update();
	void Draw();
	CVector3 Getpos()
	{
		return pos;
	}
private:
	int No = 0;
	CharacterController m_collider;						//キャラクターコントローラー
	CVector3 posm = CVector3::AxisX();
	CVector3 pos = {0.0f,0.0f,0.0f};
	SkinModel m_model;
};	

