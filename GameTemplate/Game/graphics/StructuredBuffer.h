#pragma once
#include"ShaderResourceView.h"
#include"UnorderedAccessView.h"

class StructuredBuffer : Noncopyable
{
public:
	StructuredBuffer();
	~StructuredBuffer();
	/// <summary>
	/// クリエート関数
	/// </summary>
	/// <param name="pInitData">データ</param>
	/// <param name="bufferDesc">バッファーの設定(D3D11_BUFFER_DESC)</param>
	/// <returns>bool</returns>
	bool Create(const void*pInitData, D3D11_BUFFER_DESC& bufferDesc);
	/// <summary>
	/// 本体のゲッター
	/// </summary>
	/// <returns>ID3D11Buffer</returns>
	ID3D11Buffer*& GetBody()
	{
		return m_structuredBuffer;
	}
	/// <summary>
	/// リリース関数
	/// </summary>
	void Release();
	/// <summary>
	/// SRVのゲッター
	/// </summary>
	/// <returns>ShaderResourceView</returns>
	ShaderResourceView& GetSRV()
	{
		return m_srv;
	}
private:
	ID3D11Buffer*		m_structuredBuffer = nullptr;		//バッファー
	ShaderResourceView m_srv;							//SRV
	UnorderedAccessView m_uav;							//UAV
};

