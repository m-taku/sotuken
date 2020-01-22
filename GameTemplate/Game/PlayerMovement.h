#pragma once

class Player;
class PlayerMovement : public Noncopyable
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
	Player* m_player = nullptr;		//�v���C���[�̃|�C���^�[
	float m_fallSpeed = 0.0f;			//�d�͉����x
	float m_addGravityTime = 0.0f;		//���R��������
	float m_jumpSpeed = 1000.0f;			//�W�����v�X�s�[�h
	bool IsJump = false;				//�W�����v���H
	const float GRAVITY_PARAM = 980.0f; //�d�͉����x
};

