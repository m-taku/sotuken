#pragma once
#include "PlayerCombo.h"
class Smallsword : public PlayerCombo
{
public:
	Smallsword(Player* player);
	~Smallsword();
	enum swordcombo
	{
		pull1,
		pull2,
		push1,
		push2,
		combo1,
		combo2,
		combo3,
		combo4,
		combo5,
		combo6,
		combo7,
		gerd,
		num

	};
	void SetAnimation(Player* player) override;
	void changeY() override;
	void changeB() override;
	void pullweapon()override;
	void pushweapon()override;
	void Reset()override;
private:
	int m_combo = 0;
	swordcombo nowcombo = combo1;
};

