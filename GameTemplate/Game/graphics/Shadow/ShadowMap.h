#pragma once
class ShadowMap
{

public:
	ShadowMap();
	~ShadowMap();
	enum EnMap {
		enShadowMap,
		enCascadeNear,
		enCascadeFar,
		enMapNum
	};

	void Init(int w = 4096, int h = 4096);

	void UpdateDirection(const CVector3& Direction);
	void ShadowCasterDraw();
	ID3D11ShaderResourceView** GetRenderTargetArray()
	{
		ID3D11ShaderResourceView* srv[] = {
			m_renderTarget[enShadowMap].GetShaderResourceView(),
			m_renderTarget[enCascadeNear].GetShaderResourceView(),
			m_renderTarget[enCascadeFar].GetShaderResourceView()
		};
		return srv;
	}

	void DrawToShadowCollector();
private:
	float m_shadowHight[enMapNum];
	float m_shadowWidth[enMapNum];
	RenderTarget m_renderTarget[enMapNum];
	CMatrix m_lightViewMatrix[enMapNum];
	CMatrix m_lightProjectionMatrix[enMapNum];
	PostEffect m_postEffect;

	struct SShadowCollectCB
	{
		CMatrix m_lightViewMatrix;
		CMatrix m_lightProjectionMatrix;
	};

	ConstantBuffer m_cb;
	float m_cascadePerHigh = 0.1f;
	float m_cascadePerMidle = 0.3f;
	float m_cascadePerLow = 0.6f;
};

