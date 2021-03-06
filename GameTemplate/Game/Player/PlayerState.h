#pragma once
/// <summary>
/// プレイヤーステートの基底クラス
/// </summary>
class Player;
enum State {
	StateTownMove,			//移動中
	StateQuestMove,
	StateAvoid,
	StateAttack,
	StateAttackMode,
	StateWate,
	StateHit,
	Statedeath
};
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
	/// ドロー
	/// </summary>
	virtual void Draw()
	{

	}
	virtual bool DamageAction(float damage) {
		return true;
	}
	bool DownHp(int Damage);
protected:
	Player* m_player = nullptr;				//プレイヤーのインスタンス
};

