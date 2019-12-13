#include "stdafx.h"
#include "QuestManager.h"
#include"Test_GuestManager.h"
#include "Stage/Town.h"
#include"Player.h"
#include"Stage/QuestStage.h"
#include"Enemy/Enemy.h"


QuestManager::QuestManager()
{
}


QuestManager::~QuestManager()
{
}

bool QuestManager::Start()
{
	Stage = NewGO<QuestStage>(0, "kouya");
	m_player = FindGO<Player>("player"); m_target = FindGO<Enemy>("enemy");
	return true;
}
void QuestManager::Update()
{
	m_nowtime += GetFrameDeltaTime();
	if (m_time <= m_nowtime)
	{
		NewGO<Town>(0, "town");
		NewGO<Test_GuestManager>(0, "tes");
		DeleteGO(m_target);
		DeleteGO(Stage);
		DeleteGO(this);
	}
	else {
		if (m_doun <= 0) {
			debugtime++;
			if (debugtime >= 120) {
				NewGO<Town>(0, "town");
				NewGO<Test_GuestManager>(0, "tes");
				DeleteGO(m_target);
				DeleteGO(Stage);
				DeleteGO(this);
			}
		}
	}
}