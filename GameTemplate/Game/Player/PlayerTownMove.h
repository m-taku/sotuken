#pragma once
#include "PlayerMovement.h"
#include"PlayerState.h"
class PlayerTownMove : public PlayerState
{
public:
	PlayerTownMove(Player* player);
	~PlayerTownMove();
	void Update();
	bool DamageAction(float damage);
private:
	PlayerMovement Movement;
};

