#pragma once

#include"PlayerState.h"
class Playerdeath : public PlayerState
{
public:
	Playerdeath(Player* player);
	~Playerdeath();

	void Update();
	void DamageAction(float damage);
private:
	CQuaternion rod = CQuaternion::Identity();
};

