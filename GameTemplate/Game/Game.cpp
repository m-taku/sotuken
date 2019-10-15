#include "stdafx.h"
#include "Game.h"
#include"Player.h"
#include "Stage/Town.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start() {

	NewGO<Town>(0, "base");
	NewGO<Player>(0, "player");
	return true;
}
void Game::Update()
{

}