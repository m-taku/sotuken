#include "stdafx.h"
#include "PlayerQuestMove.h"


PlayerQuestMove::PlayerQuestMove(Player* player) :PlayerState(player)
{
	Movement.SetPlayer(player);
}


PlayerQuestMove::~PlayerQuestMove()
{
}
void PlayerQuestMove::Update()
{

	Movement.DefaultMove();
}