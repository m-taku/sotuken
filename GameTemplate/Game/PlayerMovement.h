#pragma once

class Player;
class PlayerMovement : public Noncopyable
{
public:
	PlayerMovement();
	~PlayerMovement();
	void TounMove();		//デフォルトの移動処理
	void QuestMove();
	void QuestWeaponMove();
	void AttackMove();
	/*!
	*@brief プレイヤーのポインターをセット
	*/
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	
private:
	Player* m_player = nullptr;		//プレイヤーのポインター
	float m_fallSpeed = 0.0f;			//重力加速度
	float m_addGravityTime = 0.0f;		//自由落下時間
	const float GRAVITY_PARAM = 980.0f; //重力加速度
};

