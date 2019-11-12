#include "stdafx.h"
#include "QuestManager.h"
#include"Player.h"
#include"Stage/Town.h"


QuestManager::QuestManager()
{
}


QuestManager::~QuestManager()
{
}
bool QuestManager::Start()
{
//	CreateQuest();
	return true;
}
void QuestManager::Update()
{
	if (g_pad[0].IsTrigger(EnButton::enButtonRB3))
	{
		NewGO<Town>(0,"town");
		FindGO<Player>("player")->SetPosition({-300.0f,500.0f,0.0f});
	}
}