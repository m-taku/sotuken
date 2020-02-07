#pragma once

#include"PlayerState.h"
class PlayerHit : public PlayerState
{
public:
	PlayerHit(Player* player);
	~PlayerHit();
	void Update();
	void DamageAction(float damage);
private:

	State m_statenum;
};

