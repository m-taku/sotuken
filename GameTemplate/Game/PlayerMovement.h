#pragma once

class Player;
class PlayerMovement
{
public:
	PlayerMovement();
	~PlayerMovement();
	void DefaultMove();		//デフォルトの移動処理
	/*!
	*@brief プレイヤーのポインターをセット
	*/
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	
private:
	void CameraMove();
	Player* m_player = nullptr;		//プレイヤーのポインター
	float m_gravity = 0.0f;			//重力加速度
	float m_camaraAngle = 0.0f;
	float padinput_LX = 0.0f;
	float padinput_LY = 0.0f;
};

