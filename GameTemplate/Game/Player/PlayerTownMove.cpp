#include "stdafx.h"
#include "PlayerTownMove.h"


PlayerTownMove::PlayerTownMove(Player* player) :PlayerState(player)
{
	Movement.SetPlayer(player);
}


PlayerTownMove::~PlayerTownMove()
{
}
void PlayerTownMove::Update()
{

	Movement.DefaultMove();
}