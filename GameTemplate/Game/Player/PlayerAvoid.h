#pragma once
class Player;
#include"PlayerState.h"
#include "PlayerMovement.h"
class PlayerAvoid : public PlayerState
{
public:
	PlayerAvoid(Player* player);
	~PlayerAvoid();
	void Update();
	void DamageAction(float damage);
private:
	State m_statenum;
	bool m_invincible = false;
	PlayerMovement Movement;
};

