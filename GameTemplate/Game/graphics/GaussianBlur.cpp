#include "stdafx.h"
#include "GaussianBlur.h"


GaussianBlur::GaussianBlur()
{
}


GaussianBlur::~GaussianBlur()
{
	Release();
}

void GaussianBlur::Init(ID3D11ShaderResourceView* srv, float blurIntensity)
{
	//�O�̂��ߎQ�ƃJ�E���^�𑝂₷______________�O�̂���
	srv->AddRef();
	m_SRV = srv;
	//���摜�̉𑜓x���擾
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* tex;
	m_SRV->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);
	tex->Release();
	m_srvWidth = desc.Width;
	m_srvHeight = desc.Height;

	//�c���̃����_�[�^�[�Q�b�g���쐬
	m_passXRT.Create(m_srvWidth / 2, m_srvHeight, desc.Format);
	m_passYRT.Create(m_srvWidth / 2, m_srvHeight / 2, desc.Format);
	//�萔�o�b�t�@�[�̏�����
	m_cb.Create(&m_blurPalam, sizeof(m_blurPalam));
	//�`����̏�����
	m_postEffect.Init();
	//�V�F�[�_�[�̃��[�h
	m_XVS.Load("Assets/shader/gaussianblur.fx", "VSXBlur", Shader::EnType::VS);
	m_YVS.Load("Assets/shader/gaussianblur.fx", "VSYBlur", Shader::EnType::VS);
	m_PS.Load("Assets/shader/gaussianblur.fx", "PSBlur", Shader::EnType::PS);
	//�s�N�Z���V�F�[�_�[���Z�b�g
	m_postEffect.SetPS(&m_PS);
}

void GaussianBlur::InitDownSample(ID3D11ShaderResourceView * srv, int downSampleRate, float blurIntensity)
{
	//�O�̂��ߎQ�ƃJ�E���^�𑝂₷______________�O�̂���
	srv->AddRef();
	m_SRV = srv;
	//���摜�̉𑜓x���擾
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* tex;
	m_SRV->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);
	tex->Release();
	m_srvWidth = desc.Width;
	m_srvHeight = desc.Height;

	//�c���̃����_�[�^�[�Q�b�g���쐬
	m_passXRT.Create(m_srvWidth / downSampleRate, m_srvHeight, desc.Format);
	m_passYRT.Create(m_srvWidth / downSampleRate, m_srvHeight / downSampleRate, desc.Format);
	//�萔�o�b�t�@�[�̏�����
	m_cb.Create(&m_blurPalam, sizeof(m_blurPalam));
	//�`����̏�����
	m_postEffect.Init();
	//�V�F�[�_�[�̃��[�h
	m_XVS.Load("Assets/shader/gaussianblur.fx", "VSXBlur", Shader::EnType::VS);
	m_YVS.Load("Assets/shader/gaussianblur.fx", "VSYBlur", Shader::EnType::VS);
	m_PS.Load("Assets/shader/gaussianblur.fx", "PSBlur", Shader::EnType::PS);
	//�s�N�Z���V�F�[�_�[���Z�b�g
	m_postEffect.SetPS(&m_PS);
}

void GaussianBlur::InitOriginalResolusion(ID3D11ShaderResourceView * srv, float blurIntensity)
{
	//�O�̂��ߎQ�ƃJ�E���^�𑝂₷______________�O�̂���
	srv->AddRef();
	m_SRV = srv;
	//���摜�̉𑜓x���擾
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* tex;
	m_SRV->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);
	tex->Release();
	m_srvWidth = desc.Width;
	m_srvHeight = desc.Height;
	//�c���̃����_�[�^�[�Q�b�g���쐬
	m_passXRT.Create(m_srvWidth, m_srvHeight, desc.Format);
	m_passYRT.Create(m_srvWidth, m_srvHeight, desc.Format);
	//�萔�o�b�t�@�[�̏�����
	m_cb.Create(&m_blurPalam, sizeof(m_blurPalam));
	//�`����̏�����
	m_postEffect.Init();
	//�V�F�[�_�[�̃��[�h
	m_XVS.Load("Assets/shader/gaussianblur.fx", "VSXBlur", Shader::EnType::VS);
	m_YVS.Load("Assets/shader/gaussianblur.fx", "VSYBlur", Shader::EnType::VS);
	m_PS.Load("Assets/shader/gaussianblur.fx", "PSBlur", Shader::EnType::PS);
	//�s�N�Z���V�F�[�_�[���Z�b�g
	m_postEffect.SetPS(&m_PS);
}

void GaussianBlur::InitUpScaling(ID3D11ShaderResourceView * srv, int upScaleRate, float blurIntensity)
{
	//�O�̂��ߎQ�ƃJ�E���^�𑝂₷______________�O�̂���
	srv->AddRef();
	m_SRV = srv;
	//���摜�̉𑜓x���擾
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* tex;
	m_SRV->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);
	tex->Release();
	m_srvWidth = desc.Width;
	m_srvHeight = desc.Height;
	//�c���̃����_�[�^�[�Q�b�g���쐬
	m_passXRT.Create(m_srvWidth * upScaleRate, m_srvHeight * upScaleRate, desc.Format);
	m_passYRT.Create(m_srvWidth * upScaleRate, m_srvHeight * upScaleRate, desc.Format);
	//�萔�o�b�t�@�[�̏�����
	m_cb.Create(&m_blurPalam, sizeof(m_blurPalam));
	//�`����̏�����
	m_postEffect.Init();
	//�V�F�[�_�[�̃��[�h
	m_XVS.Load("Assets/shader/gaussianblur.fx", "VSXBlur", Shader::EnType::VS);
	m_YVS.Load("Assets/shader/gaussianblur.fx", "VSYBlur", Shader::EnType::VS);
	m_PS.Load("Assets/shader/gaussianblur.fx", "PSBlur", Shader::EnType::PS);
	//�s�N�Z���V�F�[�_�[���Z�b�g
	m_postEffect.SetPS(&m_PS);
}

void GaussianBlur::Execute(float distance, float division)
{
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//�����_�[�^�[�Q�b�g�̃o�b�N�A�b�v
	ID3D11RenderTargetView* buckUpRTV = nullptr;
	ID3D11DepthStencilView* buckUpDepth = nullptr;
	D3D11_VIEWPORT buckUpViewPort;
	UINT numViewPort = 1;
	UINT* pnumViewPort = &numViewPort;
	deviceContext->OMGetRenderTargets(1, &buckUpRTV, &buckUpDepth);
	deviceContext->RSGetViewports(pnumViewPort, &buckUpViewPort);
	//�d�݌v�Z
	UpdateWeight();
	float color[] = { 0.0f,0.0f,0.0f,0.0f };
	/*���R!!--------------------------------------------------------------------------------------------------------------*/
	{
		m_passXRT.Clear(color);
		m_blurPalam.distance = distance;
		m_blurPalam.division = division;
		deviceContext->UpdateSubresource(m_cb.GetBody(), 0, NULL, &m_blurPalam, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &m_cb.GetBody());
		deviceContext->PSSetConstantBuffers(0, 1, &m_cb.GetBody());
		ID3D11RenderTargetView* rtv[] = {
			m_passXRT.GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, rtv, m_passXRT.GetDepthStencilView());
		deviceContext->RSSetViewports(1, m_passXRT.GetViewPort());
		deviceContext->VSSetShaderResources(0, 1, &m_SRV);
		deviceContext->PSSetShaderResources(0, 1, &m_SRV);
		
		m_postEffect.SetVS(&m_XVS);
		m_postEffect.Draw();
	}
	/*�^�e!!--------------------------------------------------------------------------------------------------------------*/
	{
		m_passYRT.Clear(color);
		m_blurPalam.distance = distance;
		m_blurPalam.division = division;
		deviceContext->UpdateSubresource(m_cb.GetBody(), 0, NULL, &m_blurPalam, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &m_cb.GetBody());
		deviceContext->PSSetConstantBuffers(0, 1, &m_cb.GetBody());
		ID3D11RenderTargetView* rtv[] = {
			m_passYRT.GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, rtv, m_passYRT.GetDepthStencilView());
		ID3D11ShaderResourceView* srv[] = {
			m_passXRT.GetShaderResourceView()
		};
		deviceContext->VSSetShaderResources(0, 1, srv);
		deviceContext->PSSetShaderResources(0, 1, srv);

		deviceContext->RSSetViewports(1, m_passYRT.GetViewPort());
		m_postEffect.SetVS(&m_YVS);
		m_postEffect.Draw();
	}
	/*--------------------------------------------------------------------------------------------------------------------*/
	//�o�b�N�A�b�v���Ă����������_�[�^�[�Q�b�g�ɖ߂�
	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDepth);
	deviceContext->RSSetViewports(*pnumViewPort, &buckUpViewPort);
}

void GaussianBlur::Release()
{
	if (m_SRV != nullptr)
	{
		m_SRV->Release();
	}
}

void GaussianBlur::UpdateWeight()
{
	float total = 0;
	for (int i = 0; i < WEIGHT_NUM; i++) {
		m_blurPalam.weights[i] = expf(-0.5f*(float)(i*i) / m_blurIntensity);
		total += 2.0f*m_blurPalam.weights[i];
	}
	// �K�i��
	for (int i = 0; i < WEIGHT_NUM; i++) {
		m_blurPalam.weights[i] /= total;
	}
}