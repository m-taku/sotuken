#pragma once
#include"physics/PhysicsStaticObject.h"
#include "character/CharacterController.h"
class TestStage : public IGameObject
{
public:
	TestStage();
	~TestStage();
	bool Start();
	void Update();
	void Draw();
private:
	int No = 0;
	
	PhysicsStaticObject m_poa;
	CVector3 pos = { 0.0f,0.0f,0.0f };
	SkinModel m_model;
};

