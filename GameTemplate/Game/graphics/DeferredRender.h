#pragma once
class DeferredRender
{
	enum EnGBuffer {
		enGBuffer_DiffuseTexture,		//ディフューズテクスチャ
		enGBuffer_Normal,				//法線。
		enGBuffer_World,
		enGBuffer_Depth,
		enGBuffer_Silhouette,
		enGBuffer_Num,					//Gバッファの数。
	};
public:
	DeferredRender();
	~DeferredRender();
	void Init();
	void Update();
	void Draw();
	void Release();
	ID3D11ShaderResourceView* GetWorldSRV()
	{
		return m_renderTarget[enGBuffer_World].GetShaderResourceView();
	}
	ID3D11ShaderResourceView* GetDepthSRV()
	{
		return m_renderTarget[enGBuffer_Depth].GetShaderResourceView();
	}
	ID3D11ShaderResourceView* GetSilhouetteSRV()
	{
		return m_renderTarget[enGBuffer_Silhouette].GetShaderResourceView();
	}
private:
	RenderTarget m_renderTarget[enGBuffer_Num];
	PostEffect m_postEffect;
	ToneMap m_tonemap;
	Shader m_ps;
	Shader m_vs;
};

