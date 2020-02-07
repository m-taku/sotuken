#pragma once

#include"PlayerState.h"
class Playerdeath : public PlayerState
{
public:
	Playerdeath(Player* player);
	~Playerdeath();

	void Update();
	bool DamageAction(float damage);
private:
	CQuaternion rod = CQuaternion::Identity();
};

