#pragma once

class Player;
class PlayerMovement
{
public:
	PlayerMovement();
	~PlayerMovement();
	void DefaultMove();		//�f�t�H���g�̈ړ�����
	/*!
	*@brief �v���C���[�̃|�C���^�[���Z�b�g
	*/
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	
private:
	void CameraMove();
	Player* m_player = nullptr;		//�v���C���[�̃|�C���^�[
	float m_gravity = 0.0f;			//�d�͉����x
	float m_camaraAngle = 0.0f;
	float padinput_LX = 0.0f;
	float padinput_LY = 0.0f;
};

