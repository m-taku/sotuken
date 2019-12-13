#pragma once

#include"PlayerState.h"
class Playerdeath : public PlayerState
{
public:
	Playerdeath(Player* player);
	~Playerdeath();

	void Update();
private:
	CQuaternion rod = CQuaternion::Identity();
};

