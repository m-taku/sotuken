#pragma once
#include"Stage_Base.h"
#include "UI/UI.h";
#include"Route search/Navimake.h"
class QuestStage : public Stage_Base
{
public:
	QuestStage();
	~QuestStage();
	bool Init();
	void Update();
	void DrawDebug();
private:
	UI* m_ui = nullptr;
	Navimake* m_navimake = nullptr;				//ナビゲーションメッシュ作成処理。
};

