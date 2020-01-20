#pragma once
/*!
 *@brief	グラフィックスエンジン。
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
	 *@brief	初期化。
	 *@param[in]	hWnd		ウィンドウハンドル。
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	解放。
	 */
	void Release();
	/*!
	 *@brief	D3D11デバイスを取得。
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11デバイスコンテキストを取得。
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
	 *@brief	描画開始。
	 */
	void BegineRender();
	/*!
	 *@brief	描画終了。
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
	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D デバイスのターゲットとなる機能セット。
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11デバイス。
	IDXGISwapChain*			m_pSwapChain = NULL;		//スワップチェイン。
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11デバイスコンテキスト。
	ID3D11RenderTargetView* m_backBuffer = NULL;		//バックバッファ。
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//ラスタライザステート。
	ID3D11Texture2D*		m_depthStencil = NULL;		//デプスステンシル。
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//デプスステンシルビュー。
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

extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン