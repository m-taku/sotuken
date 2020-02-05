#pragma once
#include "PlayerMovement.h"
#include"PlayerState.h"
class PlayerQuestMove : public PlayerState
{
public:
	PlayerQuestMove(Player* player);
	~PlayerQuestMove();

	void Update();
	void DamageAction(float damage);
private:
	enum weapon
	{
		pull1,
		pull2,
		move
	};
	weapon m_weapon = move;
	PlayerMovement Movement;
};

