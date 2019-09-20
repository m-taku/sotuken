#pragma once
#include "character/CharacterController.h"
#include"physics/PhysicsStaticObject.h"
class Test;
class Text_Box;
class TEstNPC : public IGameObject
{
public:
	TEstNPC();
	~TEstNPC();
	bool Start();
	void Update();
	void Draw();
private:
	int No = 0;
	PhysicsStaticObject m_poa;
	//CharacterController m_collider;						//�L�����N�^�[�R���g���[���[
	CVector3 posm = CVector3::Zero();
	CVector3 pos = { 0.0f,0.0f,0.0f };
	SkinModel m_model;	
	SkinModel m_bikkuri;
	Text_Box *m_Text = nullptr;
	bool bikkuri = false;
	Test* test = nullptr;
};
