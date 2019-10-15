#include "stdafx.h"
#include "Game.h"
#include"Player.h"
#include "TestStage.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start() {

	NewGO<TestStage>(0, "base");
	NewGO<Player>(0, "player");
	return true;
}
void Game::Update()
{

}