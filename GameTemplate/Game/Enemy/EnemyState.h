#pragma once
class Enemy; 
class Player;
class EnemyState : public Noncopyable
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EnemyState(Enemy* enemypoint,Player* player);
	~EnemyState();
	/// <summary>
	/// �A�b�v�f�[�g�B
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �A�b�v�f�[�g�B
	/// </summary>
	virtual void Draw()
	{

	}
protected:
	Enemy* m_enemy = nullptr;
	Player* m_player = nullptr;
};

