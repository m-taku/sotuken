#pragma once
class Player;

#include"PlayerState.h"
class PlayerAttack : public PlayerState
{
public:
	PlayerAttack(Player* player);
	~PlayerAttack();
	void Update();
private:

};

