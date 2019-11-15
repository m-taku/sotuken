#pragma once
class Enemy;
class EnemyState : public Noncopyable
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EnemyState(Enemy* enemypoint);
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
};

