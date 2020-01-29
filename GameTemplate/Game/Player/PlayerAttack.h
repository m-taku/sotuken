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
	enum combo
	{
		combo1,
		combo2,
		combo3
	};
	combo m_combo;
	float hoge = 0.0f;
};

