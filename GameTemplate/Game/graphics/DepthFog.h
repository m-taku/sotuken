#pragma once
class DepthFog
{
public:
	DepthFog();
	~DepthFog();
	void Init();
	void Draw();
private:
	Shader m_ps;
	Shader m_copyPS;
	Shader m_vs;
	PostEffect m_postEffect;
	RenderTarget m_renderTarget;
	ConstantBuffer m_depthFogParamCB;
	ID3D11ShaderResourceView* m_srv = nullptr;
};

