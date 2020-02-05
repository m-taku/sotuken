#include "stdafx.h"
#include "NPCBase.h"
#include"Player.h"
NPCBase::NPCBase()
{
}


NPCBase::~NPCBase()
{
}	
bool NPCBase::Start() 
{
	m_player = FindGO<Player>("player");
	if (m_player != nullptr)
	{
		return Init();
	}
	return false;
}
void NPCBase::Update() {
	auto vector = m_position - m_player->GetPosition();
	if (vector.Length() <= 100.0f)
	{
		Talk();
	}
}