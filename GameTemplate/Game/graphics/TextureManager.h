#pragma once
class TextureManager : public Noncopyable
{
public:
	TextureManager();
	~TextureManager();
	/*!
	*@brief	モデルをロード。
	*@param[in]	filePath	ファイルパス。
	*@param[in]	skeleton	スケルトン。
	*/
	ID3D11ShaderResourceView* Load(const wchar_t* filePath);
	/*!
	*@brief	モデルデータを全開放。
	*/
	void Release();
private:
	//ファイルパスをキー、DirectXModelのインスタンスを値とするマップ。
	//辞書みたいなものです。
	std::map<
		std::wstring,					//wstringがキー。 
		ID3D11ShaderResourceView*   	//これが値。
	> m_directX;
};

//g_skinModelDataManagerのextern宣言。
//extern宣言は実態ではないので注意！
//コンパイラにどこかにあるから使ってねと教えるもの。
extern TextureManager g_TextureManager;