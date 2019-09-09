#pragma once
class StructuredBuffer;

class UnorderedAccessView : Noncopyable
{
public:
	UnorderedAccessView();
	~UnorderedAccessView();
	/// <summary>
	/// クリエイト関数
	/// </summary>
	/// <param name="structuredBuffer">ストラクチャードバッファー(StructuredBuffer)</param>
	/// <returns>bool</returns>
	bool Create(StructuredBuffer& structuredBuffer);
	/// <summary>
	/// クリエイト関数
	/// </summary>
	/// <param name="texture">ID3D11Texture2D&</param>
	/// <returns>bool</returns>
	bool Create(ID3D11Texture2D* texture);
	/// <summary>
	/// リリース関数
	/// </summary>
	void Release();
	/// <summary>
	/// 本体のゲッター
	/// </summary>
	/// <returns>ID3D11UnorderedAccessView</returns>
	ID3D11UnorderedAccessView*& GetBody()
	{
		return m_uav;
	}
	/// <summary>
	/// IsValidのゲッター
	/// </summary>
	/// <returns>bool</returns>
	bool IsValid() const
	{
		return m_isValid;
	}
private:
	ID3D11UnorderedAccessView*	m_uav = nullptr;	//UAV
	bool						m_isValid = false;	//有効かどうかのフラグ
};

