#pragma once
class Player;
#include"PlayerState.h"
class PlayerAttackMode : public PlayerState
{
public:
	PlayerAttackMode(Player* player);
	~PlayerAttackMode();
	void Update();

private:
	enum weapon
	{
		push1,
		push2,
		move
	};

	weapon m_weapon = move;
};

