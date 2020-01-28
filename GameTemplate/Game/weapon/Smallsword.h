#pragma once
#include "PlayerCombo.h"
class Smallsword : public PlayerCombo
{
public:
	Smallsword();
	~Smallsword();
	enum swordcombo
	{
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
	void Reset()override;
private:
	int m_combo = 0;
	swordcombo nowcombo = combo1;
};

