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
	ID3D11Texture2D*			m_renderTargetTex = nullptr;	//�����_�[�^�[�Q�b�g�̃e�N�X�`��
	ID3D11RenderTargetView*		m_renderTargetView = nullptr;	//�����_�[�^�[�Q�b�g�r���[
	ID3D11ShaderResourceView*	m_renderTargetSRV = nullptr;	//�����_�[�^�[�Q�b�g��SRV
	ID3D11Texture2D*			m_depthStencilTex = nullptr;	//�f�v�X�X�e���V���̃e�N�X�`��
	ID3D11DepthStencilView*		m_depthStencilView = nullptr;	//�f�v�X�X�e���V���r���[
	D3D11_VIEWPORT				m_viewPort;						//�r���[�|�[�g
	DXGI_FORMAT					m_textureFormat = DXGI_FORMAT_UNKNOWN;			//<�e�N�X�`���̃t�H�[�}�b�g�B
	DXGI_FORMAT					m_depthStencilFormat = DXGI_FORMAT_UNKNOWN;		//<�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g�B
	int m_width = 0;
	int m_height = 0;

};

