#include "stdafx.h"
#include "dorakomesu.h"
#include"Player.h"
#include"Enemy/Enemy.h"
dorakomesu::dorakomesu(CVector3 position) : monster(position)
{
}


dorakomesu::~dorakomesu()
{
}
void dorakomesu::attack()
{
	//FindGO<Player>("player")->Hp--;
}
void dorakomesu::Init()
{

}