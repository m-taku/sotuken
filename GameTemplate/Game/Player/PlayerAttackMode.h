#pragma once
class Player;
#include "PlayerMovement.h"
#include"PlayerState.h"
class PlayerAttackMode : public PlayerState
{
public:
	PlayerAttackMode(Player* player);
	~PlayerAttackMode();
	void Update();
	bool DamageAction(float damage);
private:
	enum weapon
	{
		push1,
		push2,
		move
	};

	weapon m_weapon = move;
	PlayerMovement Movement;
};

