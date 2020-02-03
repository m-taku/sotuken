#include "stdafx.h"
#include "PlayerAttack.h"
#include"Player.h"
#include"../HitManeger.h"
PlayerAttack::PlayerAttack(Player* player) :PlayerState(player)
{

	Movement.SetPlayer(player);
}


PlayerAttack::~PlayerAttack()
{
}
void PlayerAttack::Update()
{
	if (m_player->IsAnimEvent(1)) {
		auto Mat = m_player->GetBone(L"mixamorig:RightHand").GetWorldMatrix();
		CVector3 pos;
		pos.x = Mat.v[3].x;
		pos.y = Mat.v[3].y;
		pos.z = Mat.v[3].z;
		GetHitObjict().HitTest(pos, 80.0f, m_player->GetAttack(), HitObject::enemy);
	}
	if (!m_player->IsPlayinganim()) {
		m_player->TransitionState(Player::StateAttackMode);
	}
	else {
		Movement.AttackMove();
		if (m_player->IsAnimEvent(2)) {
			if (g_pad[0].IsTrigger(enButtonY)) {
				m_player->Getcombo()->changeY();
				m_player->TransitionState(Player::StateAttack);
			}
			else if (g_pad[0].IsTrigger(enButtonB)) {
				m_player->Getcombo()->changeB();
				m_player->TransitionState(Player::StateAttack);
			}
		}
	}
}
