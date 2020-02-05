#pragma once
class DeferredRender
{
	enum EnGBuffer {
		enGBuffer_DiffuseTexture,		//�f�B�t���[�Y�e�N�X�`��
		enGBuffer_Normal,				//�@���B
		enGBuffer_World,
		enGBuffer_Depth,
		enGBuffer_Silhouette,
		enGBuffer_Num,					//G�o�b�t�@�̐��B
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

