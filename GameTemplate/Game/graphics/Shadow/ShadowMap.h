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

	void Init(int w = 1024, int h = 1024);

	void UpdateDirection(const CVector3& Direction);
	void ShadowCasterDraw();

	void DrawToShadowCollector();
private:
	float m_shadowHight[enMapNum];
	float m_shadowWidth[enMapNum];
	RenderTarget m_renderTarget[enMapNum];
	CMatrix m_lightViewMatrix[enMapNum];
	CMatrix m_lightProjectionMatrix[enMapNum];
	PostEffect m_postEffect;

	Shader m_preCollectPS;
	Shader m_copyPS;
	Shader m_collectPS;
	Shader m_vs;

	struct SShadowCollectCB
	{
		CMatrix m_lightViewMatrix;
		CMatrix m_lightProjectionMatrix;
	};
	struct SDrawCB
	{
		CMatrix m_lightViewMatrix[enMapNum];
		CMatrix m_lightProjectionMatrix[enMapNum];
		int No;
	};
	struct camer
	{
		float width = 0.0f;
		float farZ = 0.0f;
		float height = 0.0f;
	};
	camer m_Camera[enMapNum];
	ConstantBuffer m_cb;
	ConstantBuffer m_drawCB;
	float m_cascadePerHigh = 0.1f;
	float m_cascadePerMidle = 0.3f;
	float m_cascadePerLow = 0.6f;
};

