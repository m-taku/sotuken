#pragma once
#include"EnemyState.h"
class Enemy;
class EnemyStateLoitering:public EnemyState
{
public:
	EnemyStateLoitering(Enemy* enemypoint);
	~EnemyStateLoitering();
	void Update();
private:
	CVector3 m_MoveSpeed = CVector3::Zero();
};

