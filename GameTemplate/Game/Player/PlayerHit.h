#pragma once

#include "PlayerMovement.h"
#include"PlayerState.h"
class PlayerHit : public PlayerState
{
public:
	PlayerHit(Player* player);
	~PlayerHit();
	void Update();
	bool DamageAction(float damage);
private:

	State m_statenum;
	PlayerMovement Movement;
};

