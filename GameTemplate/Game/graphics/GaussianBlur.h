#pragma once
class GaussianBlur
{
public:
	GaussianBlur();
	~GaussianBlur();
	void Init(ID3D11ShaderResourceView* srv, float blurIntensity = 25.0f);
	void InitOriginalResolusion(ID3D11ShaderResourceView* srv, float blurIntensity = 25.0f);

	void Execute(float offset = 16.0f);

	ID3D11ShaderResourceView* GetBlurSRV()
	{
		return m_passYRT.GetShaderResourceView();
	}
private:
	void UpdateWeight();
	static const int WEIGHT_NUM = 8;
	struct SBlurPalam {
		CVector4 offset = { 0.0f,0.0f,0.0f,0.0f };
		float weights[WEIGHT_NUM];
		float dist = 2.0f;
	};
	SBlurPalam m_blurPalam;
	ID3D11ShaderResourceView* m_SRV = nullptr;
	RenderTarget m_passXRT;
	RenderTarget m_passYRT;
	PostEffect m_postEffect;
	int m_srvWidth = 0.0f;
	int m_srvHeight = 0.0f;
	float m_blurIntensity = 25.0f;
	ConstantBuffer m_cb;
};

