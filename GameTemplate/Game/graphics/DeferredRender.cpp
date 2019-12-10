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
		switch (i)
		{
		case enGBuffer_Depth: {
			m_renderTarget[i].Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R32_FLOAT);
			float color[4] = { 1.0f,1.0f,1.0f,1.0f };
			m_renderTarget[i].Clear(color);
			break;
		}
		case enGBuffer_World: {
			m_renderTarget[i].Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R32G32B32A32_FLOAT);
			float color[4] = { 0.0f,0.0f,0.0f,1.0f };
			m_renderTarget[i].Clear(color);
			break;
		}
		case enGBuffer_DiffuseTexture: {
			m_renderTarget[i].Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
			float color[4] = { 0.5f,0.5f,0.5f,1.0f };
			m_renderTarget[i].Clear(color);
			break;
		}
		default: {
			m_renderTarget[i].Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
			float color[4] = { 0.0f,0.0f,0.0f,1.0f };
			m_renderTarget[i].Clear(color);
			break;
		}
		}
	}
	m_postEffect.Init();
	m_vs.Load("Assets/shader/diferredRender.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("Assets/shader/diferredRender.fx", "PSMain", Shader::EnType::PS);
	m_postEffect.SetPS(&m_ps);
	m_postEffect.SetVS(&m_vs);
	m_tonemap.Init(g_graphicsEngine->GetMainRenderTarget().GetShaderResourceView());
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
	deviceContext->RSSetViewports(1, m_renderTarget[enGBuffer_DiffuseTexture].GetViewPort());
	for (int i = 0; i < enGBuffer_Num; i++)
	{
		switch (i)
		{
		case enGBuffer_Depth: {
			float color[4] = { 1.0f,1.0f,1.0f,1.0f };
			m_renderTarget[i].Clear(color);
			break;
		}
		case enGBuffer_DiffuseTexture: {
			float color[4] = { 0.5f,0.5f,0.5f,1.0f };
			m_renderTarget[i].Clear(color);
			break;
		}
		default: {
			float color[4] = { 0.0f,0.0f,0.0f,1.0f };
			m_renderTarget[i].Clear(color);
			break;
		}
		}
	}
}

void DeferredRender::Draw()
{
	g_graphicsEngine->ChangeMainRenderTarget(true);
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ID3D11ShaderResourceView* srv[] = {
		m_renderTarget[enGBuffer_DiffuseTexture].GetShaderResourceView(),
		m_renderTarget[enGBuffer_Normal].GetShaderResourceView(),
		m_renderTarget[enGBuffer_World].GetShaderResourceView(),
		m_renderTarget[enGBuffer_Depth].GetShaderResourceView(),
		smLightManager().GetShadowSRV()
	};
	deviceContext->PSSetShaderResources(0, enGBuffer_Num+1, srv);
	deviceContext->VSSetShaderResources(0, enGBuffer_Num+1, srv);
	m_postEffect.Draw();
	m_tonemap.Draw();
}

void DeferredRender::Release()
{
}
