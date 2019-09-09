#pragma once
class IndexBuffer : Noncopyable
{
public:
	/// <summary>
	/// インデックスタイプ
	/// </summary>
	enum EnIndexType {
		enIndexType_16,
		enIndexType_32,
	};
	IndexBuffer();
	~IndexBuffer();
	/// <summary>
	/// クリエート関数
	/// </summary>
	/// <param name="numIndex">インデックス数</param>
	/// <param name="indexType">インデックスタイプ</param>
	/// <param name="pSrcIndexBuffer">インデックスデータ</param>
	/// <returns>bool</returns>
	bool Create(int numIndex, EnIndexType indexType, const void*pSrcIndexBuffer);
	/// <summary>
	/// リリース関数
	/// </summary>
	void Release();
	/// <summary>
	/// インデックスタイプのゲッター
	/// </summary>
	/// <returns>EnIndexType</returns>
	EnIndexType GetIndexType() const
	{
		return m_indexType;
	}
	/// <summary>
	/// 本体のゲッター
	/// </summary>
	/// <returns>ID3D11Buffer</returns>
	ID3D11Buffer* GetBody() const
	{
		return m_indexBuffer;
	}
	/// <summary>
	/// インデックス数のゲッター
	/// </summary>
	/// <returns></returns>
	unsigned int GetNumIndex() const
	{
		return m_numIndex;
	}
private:
	ID3D11Buffer*	m_indexBuffer = nullptr;		//インデックスバッファー
	EnIndexType		m_indexType = enIndexType_16;	//インデックスタイプ
	unsigned int	m_numIndex = 0;					//インデックス数
};

