#pragma once
class Player;
#include"PlayerState.h"
class PlayerAttackMode : public PlayerState
{
public:
	PlayerAttackMode(Player* player);
	~PlayerAttackMode();
	void Update();
};

