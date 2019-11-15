#pragma once
class Enemy;
class EnemyState : public Noncopyable
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyState(Enemy* enemypoint);
	~EnemyState();
	/// <summary>
	/// アップデート。
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// アップデート。
	/// </summary>
	virtual void Draw()
	{

	}
protected:
	Enemy* m_enemy = nullptr;
};

