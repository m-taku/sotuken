#pragma once
#include "PlayerMovement.h"
#include"PlayerState.h"
class PlayerQuestMove : public PlayerState
{
public:
	PlayerQuestMove(Player* player);
	~PlayerQuestMove();
	void Update();
private:
	PlayerMovement Movement;
};

