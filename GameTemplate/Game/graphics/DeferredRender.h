#pragma once
class DeferredRender
{
	enum EnGBuffer {
		enGBuffer_DiffuseTexture,		//ディフューズテクスチャ
		enGBuffer_Normal,				//法線。
		enGBuffer_World,
		enGBuffer_Depth,
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
private:
	RenderTarget m_renderTarget[enGBuffer_Num];
	PostEffect m_postEffect;
};

