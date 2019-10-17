#include "stdafx.h"
#include "graphics/DeferredRender.h"


DeferredRender::DeferredRender()
{
}

DeferredRender::~DeferredRender()
{
}

void DeferredRender::Init()
{
	for (int i = 0; i < enGBuffer_Num; i++)
	{
		if (i != enGBuffer_Depth)
		{
			m_renderTarget[i].Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
			float color[4] = { 0.0f,0.0f,0.0f,1.0f };
			m_renderTarget[i].Clear(color);
		}
		else
		{
			m_renderTarget[i].Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R32_FLOAT);
			float color[4] = { 1.0f,1.0f,1.0f,1.0f };
			m_renderTarget[i].Clear(color);
		}
	}
	m_postEffect.Init();
	m_postEffect.SetVS("Assets/shader/diferredRender.fx", "VSMain");
	m_postEffect.SetPS("Assets/shader/diferredRender.fx", "PSMain");
}

void DeferredRender::Update()
{
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ID3D11RenderTargetView* renderTargetView[] = {
		m_renderTarget[enGBuffer_DiffuseTexture].GetRenderTatgetView(),
		m_renderTarget[enGBuffer_Normal].GetRenderTatgetView(),
		m_renderTarget[enGBuffer_World].GetRenderTatgetView(),
		m_renderTarget[enGBuffer_Depth].GetRenderTatgetView(),
	};
	deviceContext->OMSetRenderTargets(enGBuffer_Num, renderTargetView, m_renderTarget[enGBuffer_DiffuseTexture].GetDepthStencilView());
	deviceContext->RSSetViewports(enGBuffer_Num, m_renderTarget[enGBuffer_DiffuseTexture].GetViewPort());
	for (int i = 0; i < enGBuffer_Num; i++)
	{
		if (i != enGBuffer_Depth)
		{
			float color[4] = { 0.0f,0.0f,0.0f,1.0f };
			m_renderTarget[i].Clear(color);
		}
		else
		{
			float color[4] = { 1.0f,1.0f,1.0f,1.0f };
			m_renderTarget[i].Clear(color);
		}
	}
}

void DeferredRender::Draw()
{
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	g_graphicsEngine->BegineRender();
	ID3D11ShaderResourceView* srv[] = {
		m_renderTarget[enGBuffer_DiffuseTexture].GetShaderResourceView(),
		m_renderTarget[enGBuffer_Normal].GetShaderResourceView(),
		m_renderTarget[enGBuffer_World].GetShaderResourceView(),
		m_renderTarget[enGBuffer_Depth].GetShaderResourceView(),
	};
	deviceContext->PSSetShaderResources(0, enGBuffer_Num, srv);
	deviceContext->VSSetShaderResources(0, enGBuffer_Num, srv);
	m_postEffect.Draw();
}

void DeferredRender::Release()
{
}
