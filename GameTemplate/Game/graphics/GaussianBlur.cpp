#include "stdafx.h"
#include "GaussianBlur.h"


GaussianBlur::GaussianBlur()
{
}


GaussianBlur::~GaussianBlur()
{
	if (m_SRV != nullptr)
	{
		m_SRV->Release();
	}
}

void GaussianBlur::Init(ID3D11ShaderResourceView* srv, float blurIntensity)
{
	srv->AddRef();
	m_SRV = srv;
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* tex;
	m_SRV->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);
	tex->Release();
	m_srvWidth = desc.Width;
	m_srvHeight = desc.Height;
	m_passXRT.Create(m_srvWidth / 2, m_srvHeight, desc.Format);
	m_passYRT.Create(m_srvWidth / 2, m_srvHeight / 2, desc.Format);
	m_cb.Create(&m_blurPalam, sizeof(m_blurPalam));
	m_postEffect.Init();
	m_postEffect.SetPS("Assets/shader/gaussianblur.fx", "PSBlur");
}

void GaussianBlur::InitOriginalResolusion(ID3D11ShaderResourceView * srv, float blurIntensity)
{
	srv->AddRef();
	m_SRV = srv;
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* tex;
	m_SRV->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);
	tex->Release();
	m_srvWidth = desc.Width;
	m_srvHeight = desc.Height;
	m_passXRT.Create(m_srvWidth, m_srvHeight, desc.Format);
	m_passYRT.Create(m_srvWidth, m_srvHeight, desc.Format);
	m_cb.Create(&m_blurPalam, sizeof(m_blurPalam));
	m_postEffect.Init();
	m_postEffect.SetPS("Assets/shader/gaussianblur.fx", "PSBlur");
}

void GaussianBlur::Execute(float offset)
{
	float dist = offset / 2.0f;
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ID3D11RenderTargetView* buckUpRTV = nullptr;
	ID3D11DepthStencilView* buckUpDepth = nullptr;
	D3D11_VIEWPORT buckUpViewPort;
	UINT numViewPort = 1;
	UINT* pnumViewPort = &numViewPort;
	deviceContext->OMGetRenderTargets(1, &buckUpRTV, &buckUpDepth);
	deviceContext->RSGetViewports(pnumViewPort, &buckUpViewPort);
	UpdateWeight();
	float color[] = { 0.0f,0.0f,0.0f,0.0f };
	/*--------------------------------------------------------------------------------------------------------------------*/
	{
		m_passXRT.Clear(color);
		m_postEffect.SetVS("Assets/shader/gaussianblur.fx", "VSXBlur");
		m_blurPalam.offset.x = offset / m_srvWidth;
		m_blurPalam.offset.y = 0.0f;
		m_blurPalam.dist = dist;
		deviceContext->UpdateSubresource(m_cb.GetBody(), 0, NULL, &m_blurPalam, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &m_cb.GetBody());
		deviceContext->PSSetConstantBuffers(0, 1, &m_cb.GetBody());
		deviceContext->VSSetShaderResources(0, 1, &m_SRV);
		deviceContext->PSSetShaderResources(0, 1, &m_SRV);
		ID3D11RenderTargetView* rtv[] = {
			m_passXRT.GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, rtv, m_passXRT.GetDepthStencilView());
		deviceContext->RSSetViewports(1, m_passXRT.GetViewPort());
		m_postEffect.Draw();
	}
	/*--------------------------------------------------------------------------------------------------------------------*/
	{
		m_passYRT.Clear(color);
		m_postEffect.SetVS("Assets/shader/gaussianblur.fx", "VSYBlur");
		m_blurPalam.offset.x = 0.0f;
		m_blurPalam.offset.y = offset / m_srvHeight;
		m_blurPalam.dist = dist;
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
		m_postEffect.Draw();
	}
	/*--------------------------------------------------------------------------------------------------------------------*/

	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDepth);
	deviceContext->RSSetViewports(*pnumViewPort, &buckUpViewPort);
}

void GaussianBlur::UpdateWeight()
{
	float total = 0;
	for (int i = 0; i < WEIGHT_NUM; i++) {
		m_blurPalam.weights[i] = expf(-0.5f*(float)(i*i) / m_blurIntensity);
		total += 2.0f*m_blurPalam.weights[i];
	}
	// ‹KŠi‰»
	for (int i = 0; i < WEIGHT_NUM; i++) {
		m_blurPalam.weights[i] /= total;
	}
}