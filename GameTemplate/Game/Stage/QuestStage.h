#pragma once
#include"Stage_Base.h"
#include "UI/UI.h";
class QuestStage : public Stage_Base
{
public:
	QuestStage();
	~QuestStage();
	bool Init();
	void DrawDebug();
private:
	UI* m_ui = nullptr;
};

