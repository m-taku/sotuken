#pragma once
/*!
 *@brief	�O���t�B�b�N�X�G���W���B
 */
class GraphicsEngine
{
	struct DepthFogParam {
		float depthPow = 5000.0f;
	};
public:
	GraphicsEngine();
	~GraphicsEngine();
	/*!
	 *@brief	�������B
	 *@param[in]	hWnd		�E�B���h�E�n���h���B
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	����B
	 */
	void Release();
	/*!
	 *@brief	D3D11�f�o�C�X���擾�B
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11�f�o�C�X�R���e�L�X�g���擾�B
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	ID3D11RasterizerState* GetRasterizerState()
	{
		return m_rasterizerState;
	}
	/*!
	 *@brief	�`��J�n�B
	 */
	void BegineRender();
	/*!
	 *@brief	�`��I���B
	 */
	void EndRender();

	void ChangeBuckBuffer();

	void ChangeMainRenderTarget(bool clearcolor);

	void PostEffectDraw();

	DeferredRender& GetDeferredRender()
	{
		return m_deferredRender;
	}

	RenderTarget& GetMainRenderTarget()
	{
		return m_mainRenderTarget;
	}

	void SetShadowMapHight(float hight)
	{
		m_shadowMapHight = hight;
	}

	void SetShadowAvailableLength(float length)
	{
		m_shadowAvailableLength = length;
	}

	float GetShadowMapHight()
	{
		return m_shadowMapHight;
	}
	
	float GetShadowAvailableLength()
	{
		return m_shadowAvailableLength;
	}
	void BloomDraw()
	{
		m_bloom.Draw();
	}
	void DepthFogDraw()
	{
		m_depthFog.Draw();
	}
	DepthFogParam& GetDepthFogParam()
	{
		return m_depthFogParam;
	}
private:
	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D �f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g�B
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11�f�o�C�X�B
	IDXGISwapChain*			m_pSwapChain = NULL;		//�X���b�v�`�F�C���B
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11�f�o�C�X�R���e�L�X�g�B
	ID3D11RenderTargetView* m_backBuffer = NULL;		//�o�b�N�o�b�t�@�B
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//���X�^���C�U�X�e�[�g�B
	ID3D11Texture2D*		m_depthStencil = NULL;		//�f�v�X�X�e���V���B
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//�f�v�X�X�e���V���r���[�B
	RenderTarget			m_mainRenderTarget;
	PostEffect m_posteffect;
	Shader m_ps;
	Shader m_vs;
	DeferredRender m_deferredRender;
	Bloom m_bloom;
	DepthFog m_depthFog;

	DepthFogParam m_depthFogParam;
	float m_shadowMapHight = 3000.0f;
	float m_shadowAvailableLength = 3000.0f;
};

extern GraphicsEngine* g_graphicsEngine;			//�O���t�B�b�N�X�G���W��