#include "stdafx.h"
#include"QuestSelect.h"
#include"Player.h"
#include"Stage/Town.h"
#include"Test_GuestManager.h"


QuestSelect::QuestSelect()
{
}


QuestSelect::~QuestSelect()
{

}
bool QuestSelect::Start()
{
	FindGO<Test_GuestManager>("tes")->Printkami();
	return true;
}
void QuestSelect::Update()
{
	if (g_pad[0].IsTrigger(EnButton::enButtonRB3))
	{
		//NewGO<QuestStage>(0,"town");
		//FindGO<Player>("player")->SetPosition({-300.0f,500.0f,0.0f});
		FindGO<Test_GuestManager>("tes")->CloseGuest();
		auto pul = FindGO<Player>("player");
		pul->TransitionState(Player::StateTownMove);
		//pul->SetPosition({0.0f,1000.0f,0.0f});
		//DeleteGO(FindGO<Town>("town"));
		DeleteGO(this);
	}
}