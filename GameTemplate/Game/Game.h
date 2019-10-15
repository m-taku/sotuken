#pragma once
class TestStage;
class TEstNPC;
class Player;
class Game: public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();


	TestStage* m_stage = nullptr;
	TEstNPC* m_NPC = nullptr;
	Player* m_player = nullptr;
};

