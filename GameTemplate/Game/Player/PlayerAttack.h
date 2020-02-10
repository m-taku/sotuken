#pragma once
class Player;

#include "PlayerMovement.h"
#include"PlayerState.h"
class PlayerAttack : public PlayerState
{
public:
	PlayerAttack(Player* player);
	~PlayerAttack();
	void Update();
	bool DamageAction(float damage);
private:
	enum combo
	{
		combo1,
		combo2,
		combo3
	};
	bool isHit = false;
	combo m_combo;
	float hoge = 0.0f;
	PlayerMovement Movement;
};

