#pragma once
class Enemy; 
class Player;
class EnemyState : public Noncopyable
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyState(Enemy* enemypoint,Player* player);
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
	Player* m_player = nullptr;
};

