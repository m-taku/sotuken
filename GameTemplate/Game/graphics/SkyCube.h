#pragma once
class SkyCube :Noncopyable
{
public:
	SkyCube();
	~SkyCube();
	void Init(const wchar_t* modelFilePath = L"Assets/modelData/preset/sky.cmo",
		const wchar_t* cubeMapFilePath = L"Assets/modelData/preset/skyCubeMap.dds");
	void Update(const CVector3& position, const CVector3& scale);
	void Draw();
private:
	SkinModel m_skinModel;
	ShaderResourceView m_cubeMapSRV;
	RenderTarget m_renderTarget;
	RenderTarget m_addRenderTarget;
	Shader m_copyPS;
	Shader m_copyVS;

	Shader m_addPS;
	Shader m_addVS;

	PostEffect m_fullScreenDraw;

	CMatrix ViewMatrix;
	CMatrix ProjectionMatrix;
};

