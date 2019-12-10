#include "stdafx.h"
#include "DepthFog.h"


DepthFog::DepthFog()
{
}


DepthFog::~DepthFog()
{
}

void DepthFog::Init()
{
	//Œ³‰æ‘œ‚Ì‰ð‘œ“x‚ðŽæ“¾
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* tex;
	m_srv = g_graphicsEngine->GetMainRenderTarget().GetShaderResourceView();
	m_srv->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);
	tex->Release();
	m_renderTarget.Create(desc.Width, desc.Height, desc.Format);
	m_ps.Load("Assets/shader/depthfog.fx", "PSMain", Shader::EnType::PS);
	m_copyPS.Load("Assets/shader/depthfog.fx", "PSCopyMain", Shader::EnType::PS);
	m_vs.Load("Assets/shader/depthfog.fx", "VSMain", Shader::EnType::VS);
	m_postEffect.Init();
	m_postEffect.SetVS(&m_vs);
	m_depthFogParamCB.Create(&g_graphicsEngine->GetDepthFogParam(), sizeof(g_graphicsEngine->GetDepthFogParam()));
}

void DepthFog::Draw()
{
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	deviceContext->UpdateSubresource(m_depthFogParamCB.GetBody(), 0, NULL, &g_graphicsEngine->GetDepthFogParam(), 0, 0);
	deviceContext->PSSetConstantBuffers(enDepthFogParam_Slot_No, 1, &m_depthFogParamCB.GetBody());
	float color[] = { 0.0f,0.0f,0.0f,1.0f };
	m_renderTarget.Clear(color);
	ID3D11RenderTargetView* rtv[] = {
		m_renderTarget.GetRenderTatgetView()
	};
	deviceContext->OMSetRenderTargets(1, rtv, m_renderTarget.GetDepthStencilView());
	ID3D11ShaderResourceView* srv[] = {
		m_srv,
		g_graphicsEngine->GetDeferredRender().GetDepthSRV()
	};
	deviceContext->PSSetShaderResources(0, 2, srv);
	deviceContext->VSSetShaderResources(0, 2, srv);
	m_postEffect.SetPS(&m_ps);
	m_postEffect.Draw();

	g_graphicsEngine->ChangeMainRenderTarget(true);
	ID3D11ShaderResourceView* copySrv[] = {
		m_renderTarget.GetShaderResourceView()
	};
	deviceContext->PSSetShaderResources(0, 1, copySrv);
	deviceContext->VSSetShaderResources(0, 1, copySrv);
	m_postEffect.SetPS(&m_copyPS);
	m_postEffect.Draw();
}
