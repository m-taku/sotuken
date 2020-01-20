#include "stdafx.h"
#include"StageManager.h"
#include"Stage/QuestStage.h"
#include "Stage/Town.h"


StageManager::StageManager()
{
}


StageManager::~StageManager()
{
}
void StageManager::changTown()
{
	DeleteGO(m_Stage);
	m_Stage = NewGO<Town>(0,"Town");
}

void StageManager::changQuestStage()
{
	DeleteGO(m_Stage);
	m_Stage = NewGO<QuestStage>(0, "QuestStage");
}