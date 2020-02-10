#pragma once

class Player;
class PlayerMovement : public Noncopyable
{
public:
	PlayerMovement();
	~PlayerMovement();
	void TounMove();		//�f�t�H���g�̈ړ�����
	void QuestMove();
	void QuestWeaponMove();
	void AttackMove();
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
	const float GRAVITY_PARAM = 980.0f; //�d�͉����x
};

