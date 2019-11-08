#pragma once
/// <summary>
/// プレイヤーステートの基底クラス
/// </summary>
class Player;
class Player_State :public Noncopyable
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">
	///プレイヤーのポインタ。（Player*）
	/// </param>
	Player_State(Player* player);
	/// <summary>
	/// デストラクタ（）
	/// </summary>
	virtual ~Player_State();
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
	Player* m_player = nullptr;				//プレイヤーのインスタンス
};

