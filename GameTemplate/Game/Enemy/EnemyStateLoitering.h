#pragma once
#include"EnemyState.h"

#include"Route search/Path.h"
class Enemy;
class EnemyStateLoitering:public EnemyState
{
public:
	EnemyStateLoitering(Enemy* enemypoint, Player* player);
	~EnemyStateLoitering();
	void Update();
	void rans();
	void Stop();
private:
	CVector3 m_MoveSpeed = CVector3::Zero();
	CVector3 m_oldposition = CVector3::Zero();					//1�t���[���O�̃|�W�V�����i�ǎC��Ȃǂ̔���ȂǂȂǁj
	CVector3 m_nextpos = CVector3::Zero();						//�o�H�T���ŏo�����̃|�W�V����
	Path* m_path = nullptr;
	bool ispath = false;
	float Navimake = 0.0f;
	std::thread Culling;
};

