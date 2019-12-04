#pragma once
class TEstNPC;
#include"Stage_Base.h"
class Town : public Stage_Base
{
public:
	Town();
	~Town();
	bool Init();
	void DrawDebug();
public:
	TEstNPC* TestNPC = nullptr;
};

