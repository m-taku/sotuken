#include "stdafx.h"
#include "GameManager.h"
#include"Enemy/Enemy.h"
#include "Player.h"
#include"QuestManager.h"
#include"NPCManager.h"
#include"Quest_NPC.h"
#include "UI/UI.h";
#include"../Enemyname/MonsterList.h"
GameManager::GameManager()
{

}


GameManager::~GameManager()
{
	for (auto& soundSource : m_pSoundSources)
	{
		if (soundSource != NULL)
		{
			delete soundSource;
		}
	}
}

bool GameManager::Start()
{
	if (count == 0) {
		for (auto& soundSource : m_pSoundSources)
		{
			soundSource = nullptr;
		}
		//m_NPCManager = NewGO<NPCManager>(0, "NPCManager");
		m_Stage.changTown();

		m_QuestManager = NewGO<QuestManager>(0, "QuestManager");
		m_player = NewGO<Player>(0, "player");
		m_ui = NewGO<UI>(0, "ui");
		m_ui->Init(150, 150, 50);
		m_ui->SetHP(50);
		m_pSoundSources[enTownBGM] = new CSoundSource;
		m_pSoundSources[enTownBGM]->Init(L"Assets/sound/Town.wav", "TownBGM");
		m_pSoundSources[enTownBGM]->Play(true);
	}count++;
	if (count >= 5) {
		g_graphicsEngine->GetFade()->FadeInStart();
		//m_player->TransitionState(Player::StateTownMove);
		//cheng(true);
		return true;
	}
	else
	{
		return false;
	}

}
void GameManager::Update()
{

	switch (m_gameNotify)
	{
	case NonQuestOrder:
		if (m_ChangeNotify)
		{
			m_QuestManager->ChangeOrderMode(QuestManager::Decision);
			m_QuestManager->SetActive(false);
			m_player->TransitionState(StateTownMove);
			m_ui->SetHaveQuest(UI::NothaveQuest);
			FindGO<Quest_NPC>("受付ジョー")->SetTalkState(Quest_NPC::QuestSelect);
			m_ChangeNotify = false;
			break;
		}
	case QuestOrder:
		if (m_ChangeNotify)
		{
			m_QuestManager->ChangeOrderMode(QuestManager::holdQuest);
			m_QuestManager->SetActive(true);
			m_ui->SetHaveQuest(UI::haveQuest);
			m_player->TransitionState(StateTownMove);
			FindGO<Quest_NPC>("受付ジョー")->SetTalkState(Quest_NPC::QuestCancel);
			m_ChangeNotify = false;
		}
		break;
	case QuestStart:
		if (m_ChangeNotify)
		{

			g_graphicsEngine->GetFade()->FadeOutStart();
			//}
			//if (g_pad[0].IsTrigger(enButtonB))
			//{
			//	g_graphicsEngine->GetFade()->FadeOutStart();
			//}
			count = 0;
			m_ChangeNotify = false;

		}
		if (!g_graphicsEngine->GetFade()->IsInFade())
		{
			m_QuestManager->CloseGuest();
			m_Stage.changQuestStage();
			m_player->TransitionState(StateQuestMove);
			m_player->Setweapon();
			m_ui->SetIsMap(UI::IsQuest);
			m_ui->Init(m_player->GetPlayerData().hp, m_player->GetPlayerData().stamina, m_QuestManager->GetQuestDate()->GetMAXTime());
			m_pSoundSources[enTownBGM]->Stop();
			//m_pSoundSources[enTownBGM]->Release();
			delete m_pSoundSources[enTownBGM];
			m_pSoundSources[enTownBGM] = NULL;
			m_pSoundSources[enStageBGM] = new CSoundSource;
			m_pSoundSources[enStageBGM]->Init(L"Assets/sound/Stage.wav", "StageBGM");
			m_pSoundSources[enStageBGM]->Play(true);
			ChangeNotify(InQuest);
		}
		break;
	case InQuest:
		if (m_ChangeNotify)
		{
			count++;
			if (count >= 3) {
				g_graphicsEngine->GetFade()->FadeInStart();
				//g_graphicsEngine->GetFade()->SetFadeInSpeed(3.0f);
				//}
				//if (g_pad[0].IsTrigger(enButtonB))
				//{
				//	g_graphicsEngine->GetFade()->FadeOutStart();
				//}

				m_ChangeNotify = false;
			}
		}
		//g_graphicsEngine->GetFade()->FadeInStart();
		m_ui->SetHP(m_player->GetPlayerNowParam().hp);
		m_ui->SetStamina(m_player->GetPlayerNowParam().stamina);
		m_ui->SetTimer(m_QuestManager->GetQuestDate()->GetNowTime());
		break;
	case EndQuest:
		if (m_ChangeNotify)
		{
			auto m_target = FindGO<Enemy>("enemy0");
			DeleteGO(m_target);
			m_target = FindGO<Enemy>("enemy1");
			DeleteGO(m_target);
			m_Stage.changTown();
			m_QuestManager->ChangeOrderMode(QuestManager::Decision);
			m_player->TransitionState(StateTownMove);
			m_ui->SetIsMap(UI::IsTown);
			m_pSoundSources[enStageBGM]->Stop();
			//m_pSoundSources[enStageBGM]->Release();
			delete m_pSoundSources[enStageBGM];
			m_pSoundSources[enStageBGM] = NULL;
			m_pSoundSources[enTownBGM] = new CSoundSource;
			m_pSoundSources[enTownBGM]->Init(L"Assets/sound/Town.wav", "TownBGM");
			m_pSoundSources[enTownBGM]->Play(true);
			m_ChangeNotify = false;
			count = 0;
		}
		count++;
		if (count>=3) {
			g_graphicsEngine->GetFade()->FadeInStart();
			m_gameNotify = NonQuestOrder;
		}
		break;
	default:
		break;
	}

	//auto pul = FindGO<Player>("player");
	//pul->TransitionState(Player::StateTownMove);
}
void  GameManager::Change(bool furag)
{
	if (!furag)
	{
		auto m_target = FindGO<Enemy>("enemy");
		DeleteGO(m_target);

		m_QuestManager->ChangeOrderMode(QuestManager::Decision);
		m_player->TransitionState(StateTownMove);
		m_Stage.changTown();
	}
	else
	{
		//m_QuestManager->CloseGuest();
		//m_Stage.changQuestStage();
		//m_player->TransitionState(Player::StateQuestMove);
		//m_player->Setweapon();
	}

}