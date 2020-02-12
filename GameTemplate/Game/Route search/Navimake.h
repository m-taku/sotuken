#pragma once
#include"physics/MeshCollider.h"
#include "physics/RigidBody.h"
#include"character/CharacterController.h"
#include"Path.h"
/// <summary>
/// 経路探査用のメッシュデータ。
/// </summary>
class Navimake :public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Navimake();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Navimake();
	/// <summary>
	/// Gameobjectから継承したDraw関数
	/// </summary>
	void Draw() override;
	/// <summary>
	/// ポジションを使ったバス番号検索。
	/// </summary>
	/// <param name="position">
	/// ポジション。(CVector3)
	/// </param>
	/// <returns>
	/// パス番号。（int）
	/// </returns>
	int FindPos_No(CVector3 position) const
	{
		CVector3 closepos;
		closepos.x = FLT_MAX;
		closepos.y = FLT_MAX;
		closepos.z = FLT_MAX;
		int No = 0;
		for (int i = 0; i < m_seru.size(); i++)
		{
			CVector3 pos = position - m_seru[i]->centerposition;
			if (closepos.Length() >= pos.Length())
			{
				//一番近いパスの番号を検索
				No = m_seru[i]->No;
				closepos = pos;
			}
		}
		return No;
	}
	/// <summary>
	/// 番号がわかっている場合の中心座標検索。
	/// </summary>
	/// <param name="No">
	/// パス番号。（int）
	/// </param>
	/// <returns>
	/// ポジション。（CVector3）
	/// </returns>
	CVector3 FindNo_Pos(int No) const
	{
		return m_seru[No]->centerposition;
	}
	/// <summary>
	/// パスの入った配列を使ったデバック表示用関数
	/// </summary>
	/// <param name="posudate">
	///  パス番号の入った可変長配列(std::vector(int))
	/// </param>
	void DebugVector(const std::vector<int>& posudate);
	/// <summary>
	/// 親のPasDateとコストと目的地を使ってリンクを検索する
	/// </summary>
	/// <param name="date">
	/// 親のPasDate
	/// </param>
	/// <param name="endNo">
	/// 目的地（int）
	/// </param>
	/// <param name="cost">
	/// 親のパスまでのコスト（float）
	/// </param>
	/// <returns>
	/// リンク情報
	/// </returns>
	const std::vector<Path::PasDate*> FindLinc(Path::PasDate& date, int endNo, float cost) const;
	/// <summary>
	/// スムーズ処理の時の地形との当たり判定
	/// </summary>
	/// <param name="sturtNo">
	/// レイを飛ばす始点
	/// </param>
	/// <param name="nextNo">
	/// レイを飛ばす終点
	/// </param>
	/// <returns>
	/// 衝突でtrue、未衝突でfalse
	/// </returns>
	bool CollisionTest(int sturtNo, int nextNo);
	void CreateRigidBody(float radius, float height, const CVector3& position);
private:
	CVector3 Searchcenter(const CVector3(&pos)[3]);	//中点を求める関数
	/// <summary>
	/// Cell構造体
	/// </summary>
	/// <remarks>
	/// ナビゲーションメッシュの最小単位となCell構造体。
	/// セルを構成する３角形の頂点と、隣接Cell情報などを持っている。
	/// </remarks>
	struct SData {
		//CVector3				normal;				//法線
		CVector3				position[3];		//三角形1個の座標
		CVector3				centerposition;		//中心座標
		int	                    linkNoList[3];		//リンクのリスト
		int                     No;					//自分の番号
		float                   cost[3];			//リンク先に行く際のコスト
	};
	MeshCollider m_meshCollider;					//メッシュ情報
	CapsuleCollider		m_collider;					//コライダー。
	SkinModel m_model;								//モデルデータ
	std::vector<SData*> m_seru;						//三角形１つのデータ
	RigidBody m_rigidBody;			               	//剛体。
	static const int hight = 80;					//CollisionTest用のカプセルの幅
	static const int ballsize = 500;				//CollisionTest用のカプセルの高さ
};