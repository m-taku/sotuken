#pragma once
class ToneMap
{
public:
	ToneMap();
	~ToneMap();
	void Init(ID3D11ShaderResourceView* srv);
	void Draw();
private:
	RenderTarget m_avelumiRT;
	RenderTarget m_finalRT;
	ID3D11ShaderResourceView* m_srv = nullptr;
	PostEffect m_postEffect;
	Shader m_finalPS;
	Shader m_vs;
	Shader m_luminancePS;
	Shader m_copyPS;
	int m_srvWidth = 0;
	int m_srvHeight = 0;
	GaussianBlur m_blur;
};

