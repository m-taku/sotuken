#pragma once
class Quest_NPC;
#include"Stage_Base.h"
class Town : public Stage_Base
{
public:
	Town();
	~Town();
	bool Init();
	void DrawDebug();
public:
	Quest_NPC* TestNPC = nullptr;
};

