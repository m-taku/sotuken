#pragma once
class RenderTarget :Noncopyable
{
public:
	RenderTarget();
	~RenderTarget();
	bool Create(int w, int h, DXGI_FORMAT colorFormat, DXGI_FORMAT depthFormat = DXGI_FORMAT_D32_FLOAT);
	void Clear(float* color);
	ID3D11Texture2D* GetRenderTargetTexture()
	{
		return m_renderTargetTex;
	}
	ID3D11RenderTargetView* GetRenderTatgetView()
	{
		return m_renderTargetView;
	}
	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return m_renderTargetSRV;
	}
	ID3D11Texture2D* GetTexture2D()
	{
		return m_depthStencilTex;
	}
	ID3D11DepthStencilView* GetDepthStencilView()
	{
		return m_depthStencilView;
	}

	D3D11_VIEWPORT* GetViewPort()
	{
		return &m_viewPort;
	}

	void Release();

private:
	ID3D11Texture2D*			m_renderTargetTex = nullptr;	//レンダーターゲットのテクスチャ
	ID3D11RenderTargetView*		m_renderTargetView = nullptr;	//レンダーターゲットビュー
	ID3D11ShaderResourceView*	m_renderTargetSRV = nullptr;	//レンダーターゲットのSRV
	ID3D11Texture2D*			m_depthStencilTex = nullptr;	//デプスステンシルのテクスチャ
	ID3D11DepthStencilView*		m_depthStencilView = nullptr;	//デプスステンシルビュー
	D3D11_VIEWPORT				m_viewPort;						//ビューポート
	DXGI_FORMAT					m_textureFormat = DXGI_FORMAT_UNKNOWN;			//<テクスチャのフォーマット。
	DXGI_FORMAT					m_depthStencilFormat = DXGI_FORMAT_UNKNOWN;		//<デプスステンシルバッファのフォーマット。
	int m_width = 0;
	int m_height = 0;

};

