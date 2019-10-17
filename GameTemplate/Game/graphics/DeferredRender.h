#pragma once
class DeferredRender
{
	enum EnGBuffer {
		enGBuffer_DiffuseTexture,		//�f�B�t���[�Y�e�N�X�`��
		enGBuffer_Normal,				//�@���B
		enGBuffer_World,
		enGBuffer_Depth,
		enGBuffer_Num,					//G�o�b�t�@�̐��B
	};
public:
	DeferredRender();
	~DeferredRender();
	void Init();
	void Update();
	void Draw();
	void Release();
private:
	RenderTarget m_renderTarget[enGBuffer_Num];
	PostEffect m_postEffect;
};
