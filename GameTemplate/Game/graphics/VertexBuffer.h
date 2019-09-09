#pragma once
class VertexBuffer : Noncopyable
{
public:
	VertexBuffer();
	~VertexBuffer();
	/// <summary>
	/// クリエート関数
	/// </summary>
	/// <param name="numVertex">頂点数</param>
	/// <param name="stride">バイトの幅size</param>
	/// <param name="pSrcVertexBuffer">頂点バッファーデータ</param>
	/// <returns>bool</returns>
	bool Create(int numVertex, int stride, const void* pSrcVertexBuffer);
	/// <summary>
	/// リリース関数
	/// </summary>
	void Release();
	/// <summary>
	/// ストライドのゲッター
	/// </summary>
	/// <returns></returns>
	int GetStride() const
	{
		return m_stride;
	}
	/// <summary>
	/// 本体のゲッター
	/// </summary>
	/// <returns>ID3D11Buffer</returns>
	ID3D11Buffer*& GetBody()
	{
		return m_vertexBuffer;
	}

private:
	ID3D11Buffer*	m_vertexBuffer = nullptr;		//頂点バッファー
	int				m_stride = 0;					//バイトの幅
};

