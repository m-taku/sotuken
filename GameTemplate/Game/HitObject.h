#pragma once
/// <summary>
/// 当たり判定用のオブジェクトデータ
/// </summary>
class HitObject :public Noncopyable
{
public:
	/// <summary>
	/// オブジェクトの名前
	/// （タグ付の目的）
	/// </summary>
	enum objict_Name {
		enemy,
		player,
		name_num
	};
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HitObject();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~HitObject();
	/// <summary>
	/// オブジェクトを制作する
	/// </summary>
	/// <param name="pos">
	/// ポジションのポインタ
	/// （動かす対象のポインタ）
	/// </param>
	/// <param name="radius">
	/// オブジェクトの半径
	/// </param>
	/// <param name="objict">
	/// フック関数
	/// </param>
	/// <param name="name">
	/// オブジェクトの名前
	/// </param>
	void Create(const CVector3* pos, float radius, std::function<void(float damage, CVector3 ObjDate)>  objict, objict_Name name);
	/// <summary>
	/// オブジェクト同士の当たり判定
	/// </summary>
	/// <param name="pos">
	/// 当てる側のポジション
	/// </param>
	/// <param name="Circle">
	/// 当てるが側の球体の半径
	/// </param>
	/// <param name="damage">
	/// 当たった場合の与える変数
	/// （今回はダメージ数）
	/// </param>
	/// <returns>
	/// 当たればtrue
	/// </returns>
	bool HitTest(CVector3 originpos,CVector3 pos, float Circle, float damage);
	/// <summary>
	/// オブジェクトの名前（タグ）のゲット
	/// </summary>
	/// <returns>
	/// オブジェクトの名前、タグ（objict_Name ）
	/// </returns>
	objict_Name Getobjict_Name()
	{
		return m_name;
	}
	const CVector3 Getpos()
	{
		if (m_pos != nullptr) {
			return *m_pos;
		}
		else
		{
			return CVector3::Zero();
		}
	}
private:
	const CVector3* m_pos = nullptr;						//オブジェクトのポジション
	float m_radius = 0.0f;									//オブジェクトの半径
	objict_Name m_name = name_num;							//オブジェクトの名前（タグ）
	std::function<void(float damage, CVector3 ObjDate)>  m_fuk;				//当たった場合に呼びたい関数
};