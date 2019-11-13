#pragma once
#include"Stage_Base.h"
class QuestStage : public Stage_Base
{
public:
	QuestStage();
	~QuestStage();
	bool Init();
	void DrawDebug();
};

