#pragma once
#ifdef  _DEBUG
/// <summary>
/// デバック用のベクトル表示クラス
/// </summary>
class VectorDraw:Noncopyable
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	/// <param name="position">
	/// ベクトルの始点。(CVector3)
	/// </param>
	/// <param name="counnt">
	/// ベクトルを何個表示するか。（int）
	/// （インスタンシングのため。後から変更不可）
	/// </param>
	VectorDraw(CVector3 position,int counnt=1)
	{
		m_vector.Init(L"Assets/modelData/vectormodel.cmo");
		m_position = position;
		m_count = counnt;
	}
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~VectorDraw();
	/// <summary>
	///  1つ1つ個別でベクトルを表示
	/// </summary>	
	/// <param name="posotion">
	/// ベクトルの始点(CVector3)
	/// </param>
	/// <param name="Vector">
	/// 可視化したいベクトル(CVector3)
	/// </param>
	/// <param name="power">
	/// ベクトルの力（長さ）（float）
	/// </param>
	void Update(CVector3 posotion, CVector3 Vector, float power);
	/// <summary>
	/// 1つ1つ個別で点を表示
	/// </summary>
	void Update(CVector3 posotion);					
	/// <summary>
	/// インスタンシングによる大量のベクトル表示
	/// </summary>
	/// <param name="posotion">
	/// ベクトルの始点の配列(std::vector(CVector3)::iterator)
	/// </param>
	/// <param name="Vector">
	/// 可視化したいベクトルの配列(std::vector(CVector3)::iterator&)
	/// </param>
	/// <param name="power">
	/// ベクトルの力（長さ）の配列（std::vector(float)::iterator）
	/// </param>
	void Update(
			std::vector<CVector3>::iterator posotion,
			std::vector<CVector3>::iterator& Vector,
			std::vector<float>::iterator power);
	/// <summary>
	/// インスタンシングによる大量の点を表示
	/// </summary>
	/// <param name="posotion">
	/// 表示したい点の配列（std::vector(CVector3)&）
	/// </param>
	void Update(const std::vector<CVector3>& posotion);
	/// <summary>
	/// 表示用の関数
	/// （勝手に呼ばれないので注意！！）
	/// </summary>
	void Draw();
private:
	SkinModel m_vector;							//スキンモデルのインスタンス
	CVector3 vector = CVector3::Zero();			//表示したいベクトルの向き（ノーマライズ）
	CVector3 m_position;		                //表示したいベクトルの原点
	int m_count = 1;							//表示するベクトル（点）の個数
	float Power = 0.0f;							//表示したいベクトルの大きさ（float）
};
#else

class VectorDraw :Noncopyable
{
public:
	
	VectorDraw(CVector3 position, int counnt = 1){}
	~VectorDraw() {}
	void Update(CVector3 posotion, CVector3 Vector, float power) {}
	void Update(CVector3 posotion) {}
	void Update(
		std::vector<CVector3>::iterator posotion,
		std::vector<CVector3>::iterator& Vector,
		std::vector<float>::iterator power) {}
	void Update(const std::vector<CVector3>& posotion) {}
	void Draw() {}
};
#endif

