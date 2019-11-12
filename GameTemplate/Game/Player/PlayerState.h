#pragma once
/// <summary>
/// プレイヤーステートの基底クラス
/// </summary>
class Player;
class PlayerState :public Noncopyable
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">
	///プレイヤーのポインタ。（Player*）
	/// </param>
	PlayerState(Player* player);
	/// <summary>
	/// デストラクタ（）
	/// </summary>
	virtual ~PlayerState();
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

