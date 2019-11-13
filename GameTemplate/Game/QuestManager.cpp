#include "stdafx.h"
#include "QuestManager.h"
#include"Player.h"
#include"Stage/Town.h"
#include"Stage/QuestStage.h"
#include"Test_GuestManager.h"


QuestManager::QuestManager()
{
}


QuestManager::~QuestManager()
{
}
bool QuestManager::Start()
{
	FindGO<Test_GuestManager>("tes")->Printkami();
		return true;
}
void QuestManager::Update()
{
	if (g_pad[0].IsTrigger(EnButton::enButtonRB3))
	{
		NewGO<QuestStage>(0,"town");
		FindGO<Player>("player")->SetPosition({-300.0f,500.0f,0.0f});
		FindGO<Test_GuestManager>("tes")->Printkami1();
		auto pul = FindGO<Player>("player");
		pul->TransitionState(Player::StateTownMove);
		pul->SetPosition({0.0f,1000.0f,0.0f});
		DeleteGO(FindGO<Town>("town"));
		DeleteGO(this);
	}
}