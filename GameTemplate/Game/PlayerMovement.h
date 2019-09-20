#pragma once

class Player;
class PlayerMovement
{
public:
	PlayerMovement();
	~PlayerMovement();
	void DefaultMove();
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
private:
	Player* m_player = nullptr;
	float m_gravity = 0.0f;
};

