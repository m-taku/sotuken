#pragma once
#include"PlayerState.h"
class PlayerWait : public PlayerState
{
public:
	PlayerWait(Player* player);
	~PlayerWait();
	void Update();
};

