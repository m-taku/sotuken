#include "stdafx.h"
#include "monster.h"
#include"Enemy/Enemy.h"
#include"Player.h"


monster::monster(CVector3 pos)
{
	m_enemy = NewGO<Enemy>(0,"enemy");
	m_enemy->SetPosition(pos);
	m_enemy->m_monster = this;
	m_player = FindGO<Player>("player");
}

bool monster::Alignment(CVector3 pos)
{
	if (m_enemy->IsAnimEvent(1))
	{
		m_jikuawase = wait;
	}
	static CVector3 jiku;
	static CVector3 vector;
	CQuaternion kaku;
	int i = 0;
	auto enemyForwaed = m_enemy->GetForward();
	switch (m_jikuawase)
	{
	case monster::strat: 
		vector = pos - m_enemy->GetPosition();
		vector.y = 0.0f;
		vector.Normalize();
		enemyForwaed.y = 0.0f;
		enemyForwaed.Normalize();
		angle = vector.Dot(enemyForwaed);
		angle = acos(angle);
		if (CMath::RadToDeg(angle) <= 10.0f|| m_count>=3)
		{
			m_count = 0;
			return true;
		}	
		else {
			jiku.Cross(enemyForwaed, vector);
			jiku.Normalize();
			kaitennkaku = min(CMath::RadToDeg(angle), 90.0f);
			kaku.SetRotationDeg(jiku, kaitennkaku * GetFrameDeltaTime()*2.0f);
			m_enemy->m_rotation.Multiply(kaku);
			m_count++;
			m_jikuawase = kaiten;
			return false;
		}
		break;
	case monster::kaiten:
		kaku.SetRotationDeg(jiku, kaitennkaku * GetFrameDeltaTime() * 2.0f);
		m_enemy->m_rotation.Multiply(kaku);
		enemyForwaed.y = 0.0f;
		enemyForwaed.Normalize();
		angle = vector.Dot(enemyForwaed);
		angle = acos(angle);
		if (CMath::RadToDeg(angle) <= 10.0f)
		{
			
			m_jikuawase = strat;
			//return true;
		}
		return false;
		break;
	case wait:
		return true;
		i++;
		i++;
		i++;
		i++;
		i++;
		break;
	default:
		break;
	}
	
}
