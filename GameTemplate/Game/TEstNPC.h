#pragma once
#include "character/CharacterController.h"
#include"physics/PhysicsStaticObject.h"
class Test;
class Player;
class Text_Box;
class TEstNPC : public IGameObject
{
public:
	TEstNPC();
	~TEstNPC();
	bool Start();
	void Update();
	void Draw();
	void SetPos(CVector3 position)
	{
		pos = position;
	}
private:
	int No = 0;
	CharacterController m_collider;						//キャラクターコントローラー
	CVector3 posm = CVector3::Zero();
	CVector3 pos = { 0.0f,0.0f,0.0f };
	SkinModel m_model;
	SkinModel m_bikkuri;
	Text_Box* m_Text[2] = {nullptr};
	bool bikkuri = false;
	Player* test = nullptr;
};

