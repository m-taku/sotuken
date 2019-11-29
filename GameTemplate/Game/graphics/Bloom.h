#pragma once
class Bloom
{
public:
	Bloom();
	~Bloom();
	//初期化
	void Init(ID3D11ShaderResourceView* srv);
	//描画
	void Draw();
	//気にしない
	void Release();
private:
	static const int DOWN_SAMPLE_NUM = 4;				//ダウンサンプル回数
	RenderTarget m_luminanceRT;							//輝度抽出用レンダーターゲット
	RenderTarget m_mixBlurTexRT;						//ボケ画像合成用レンダーターゲット
	ID3D11ShaderResourceView* m_srcTexture = nullptr;	//元画像のSRV;
	GaussianBlur m_blur[DOWN_SAMPLE_NUM];				//ブラー配列
	GaussianBlur m_finalBlur;							//ボケ画像の品質を上げるためのブラー
	ID3D11BlendState* m_disableBlendState = nullptr;	//加算合成無効
	ID3D11BlendState* m_finalBlendState = nullptr;		//加算合成有効
	PostEffect m_postEffect;	//フルスクリーン描画用
	Shader m_luminancePS;		//輝度抽出用ピクセルシェーダー
	Shader m_luminanceVS;		//輝度抽出用頂点シェーダー
	Shader m_mixBlurPS;			//ボケ画像合成用ピクセルシェーダー
	Shader m_mixBlurVS;			//ボケ画像合成用頂点シェーダー
	Shader m_finalBlendPS;		//加算合成用ピクセルシェーダー
	Shader m_finalBlendVS;		//加算合成用頂点シェーダー
	int m_srcTexWidth = 0;		//元画像の横幅
	int m_srcTexHeight = 0;		//元画像の縦幅
};

