#pragma once
class GaussianBlur
{
public:
	GaussianBlur();
	~GaussianBlur();
	void Init(ID3D11ShaderResourceView* srv, float blurIntensity = 25.0f);
	void InitDownSample(ID3D11ShaderResourceView* srv, int downSampleRate = 2, float blurIntensity = 25.0f);
	void InitOriginalResolusion(ID3D11ShaderResourceView* srv, float blurIntensity = 25.0f);
	void InitUpScaling(ID3D11ShaderResourceView* srv, int upScaleRate = 2, float blurIntensity = 25.0f);

	void Execute(float distance = 2.0f, float division = 1.0f);

	ID3D11ShaderResourceView* GetBlurSRV()
	{
		return m_passYRT.GetShaderResourceView();
	}

	void Release();
private:
	void UpdateWeight();				//重みの計算
	static const int WEIGHT_NUM = 8;
	//パラメーターの構造体
	struct SBlurPalam {

		float weights[WEIGHT_NUM];	//重み
		float distance = 2.0f;		//ピクセル間の距離
		float division = 1.0f;		//微調整用(distanceが変動する)
	};
	SBlurPalam m_blurPalam;			//パラメーターのインスタンス
	ID3D11ShaderResourceView* m_SRV = nullptr;		//元画像のSRV
	RenderTarget m_passXRT;		//Xブラー用のレンダーターゲット
	RenderTarget m_passYRT;		//Yブラー用のレンダーターゲット
	PostEffect m_postEffect;	//描くやつ
	Shader m_XVS;				//Xブラー用の頂点シェーダー
	Shader m_YVS;				//Yブラー用の頂点シェーダー
	Shader m_PS;				//ピクセルシェーダー
	int m_srvWidth = 0.0f;		//元画像の横幅
	int m_srvHeight = 0.0f;		//元画像の縦幅
	float m_blurIntensity = 25.0f;	//ブラー強度
	ConstantBuffer m_cb;			//パラメーター用の定数バッファー
};

