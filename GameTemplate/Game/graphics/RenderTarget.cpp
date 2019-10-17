#include "stdafx.h"
#include "RenderTarget.h"


RenderTarget::RenderTarget()
{
}


RenderTarget::~RenderTarget()
{
	Release();
}

bool RenderTarget::Create(int w, int h, DXGI_FORMAT colorFormat, DXGI_FORMAT depthFormat)
{
	m_width = w;
	m_height = h;
	m_textureFormat = colorFormat;
	m_depthStencilFormat = depthFormat;

	HRESULT hr;
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();

	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.Format = colorFormat;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.ArraySize = 1;
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Quality = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	hr = d3dDevice->CreateTexture2D(&texDesc, nullptr, &m_renderTargetTex);

	d3dDevice->CreateRenderTargetView(m_renderTargetTex, NULL, &m_renderTargetView);
	D3D11_TEXTURE2D_DESC depthTexDesc = texDesc;

	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = d3dDevice->CreateShaderResourceView(m_renderTargetTex, &srvDesc, &m_renderTargetSRV);

	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.Format = depthFormat;
	hr = d3dDevice->CreateTexture2D(&depthTexDesc, nullptr, &m_depthStencilTex);

	depthStencilViewDesc.Format = depthTexDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	depthStencilViewDesc.Flags = 0;
	hr = d3dDevice->CreateDepthStencilView(m_depthStencilTex, &depthStencilViewDesc, &m_depthStencilView);

	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = w;
	m_viewPort.Height = h;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	return hr;
}

void RenderTarget::Clear(float * color)
{
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->ClearRenderTargetView(m_renderTargetView, color);
	d3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderTarget::Release()
{
	if (m_renderTargetTex != nullptr)
	{
		m_renderTargetTex->Release();
	}
	if (m_renderTargetView != nullptr)
	{
		m_renderTargetView->Release();
	}
	if (m_renderTargetSRV != nullptr)
	{
		m_renderTargetSRV->Release();
	}
	if (m_depthStencilTex != nullptr)
	{
		m_depthStencilTex->Release();
	}
	if (m_depthStencilView != nullptr)
	{
		m_depthStencilView->Release();
	}
}
