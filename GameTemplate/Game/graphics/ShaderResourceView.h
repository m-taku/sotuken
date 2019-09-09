#pragma once
class StructuredBuffer;
class TextureData;
class ShaderResourceView : Noncopyable
{
public:
	ShaderResourceView();
	~ShaderResourceView();
	/// <summary>
	/// クリエート関数
	/// </summary>
	/// <param name="structuredBuffer">ストラクチャードバッファー</param>
	/// <returns></returns>
	bool Create(StructuredBuffer& structuredBuffer);
	/// <summary>
	/// クリエイト関数
	/// </summary>
	/// <param name="texture"></param>
	/// <returns></returns>
	bool Create(ID3D11Texture2D* texture);
	/// <summary>
	/// ファイルからddsを作成する関数
	/// </summary>
	/// <param name="fileName">ファイルパス(L"filepath")</param>
	/// <returns>bool</returns>
	bool CreateFromDDSTextureFromFile(const wchar_t* fileName);
	/// <summary>
	/// リリース関数
	/// </summary>
	void Release();
	/// <summary>
	/// 本体のゲッター
	/// </summary>
	/// <returns>ID3D11ShaderResourceView</returns>
	ID3D11ShaderResourceView*& GetBody()
	{
		return m_srv;
	}
	/// <summary>
	/// m_isValidのゲッター
	/// </summary>
	/// <returns></returns>
	bool IsValid() const
	{
		return m_isValid;
	}
	/// <summary>
	/// テクスチャー設定
	/// </summary>
	/// <param name="desc">設定(D3D11_TEXTURE2D_DESC)</param>
	void GetTextureDesc(D3D11_TEXTURE2D_DESC& desc) const
	{
		ID3D11Texture2D* tex;
		m_srv->GetResource((ID3D11Resource**)&tex);
		tex->GetDesc(&desc);
		tex->Release();
	}
private:
	ID3D11ShaderResourceView* m_srv = nullptr;		//SRV
	bool m_isValid = false;		//有効かどうかのフラグ
};

