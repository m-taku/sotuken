#pragma once
#include"Route search/Path.h"
#include"EnemyState.h"
class Enemy;
class Player;
class EnemyStateChase : public EnemyState
{
public:
	EnemyStateChase(Enemy* enemypoint, Player* player);
	~EnemyStateChase();
	void Update();
	void rans(CVector3 pos);
	void Stop();

private:		
	CVector3 m_oldposition = CVector3::Zero();					//1フレーム前のポジション（壁擦りなどの判定などなど）
	CVector3 m_nextpos = CVector3::Zero();						//経路探査で出た次のポジション
	Path* m_path = nullptr;
	bool ispath = false;
	float Navimake = 0.0f;
	std::thread Culling;
};

