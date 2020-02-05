#include "stdafx.h"
#include "ToneMap.h"


ToneMap::ToneMap()
{
}


ToneMap::~ToneMap()
{
	if (m_srv != nullptr)
	{
		m_srv->Release();
	}
}

void ToneMap::Init(ID3D11ShaderResourceView* srv)
{
	srv->AddRef();
	m_srv = srv;
	m_blur.InitDownSample(m_srv,64.0f);
	//元画像の解像度を取得
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* tex;
	m_srv->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);
	tex->Release();
	m_srvWidth = desc.Width;
	m_srvHeight = desc.Height;
	m_avelumiRT.Create(1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_finalRT.Create(m_srvWidth, m_srvHeight, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_postEffect.Init();
	m_luminancePS.Load("Assets/shader/tonemap.fx", "PSLuminanceMain", Shader::EnType::PS);
	m_finalPS.Load("Assets/shader/tonemap.fx", "PSFinalMain", Shader::EnType::PS);
	m_copyPS.Load("Assets/shader/tonemap.fx", "PSCopyMain", Shader::EnType::PS);
	m_vs.Load("Assets/shader/tonemap.fx", "VSMain", Shader::EnType::VS);

}

void ToneMap::Draw()
{
	m_blur.Execute(32.0f);
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//レンダーターゲットのバックアップ
	ID3D11RenderTargetView* buckUpRTV;
	ID3D11DepthStencilView* buckUpDSV;
	D3D11_VIEWPORT buckUpViewPort;
	UINT numViewPort = 1;
	UINT* num = &numViewPort;
	deviceContext->OMGetRenderTargets(1, &buckUpRTV, &buckUpDSV);
	deviceContext->RSGetViewports(num, &buckUpViewPort);
	ID3D11RenderTargetView* rtv[] = {
		m_avelumiRT.GetRenderTatgetView()
	};
	float color[] = { 0.0f,0.0f,0.0f,1.0f };
	m_avelumiRT.Clear(color);
	deviceContext->OMSetRenderTargets(1, rtv, m_avelumiRT.GetDepthStencilView());
	deviceContext->RSSetViewports(1, m_avelumiRT.GetViewPort());
	ID3D11ShaderResourceView* bokesrv[] = {
		m_blur.GetBlurSRV()
	};

	deviceContext->VSSetShaderResources(0, 1, bokesrv);
	deviceContext->PSSetShaderResources(0, 1, bokesrv);
	m_postEffect.SetPS(&m_luminancePS);
	m_postEffect.SetVS(&m_vs);
	m_postEffect.Draw();

	ID3D11RenderTargetView* finalrtv[] = {
		m_finalRT.GetRenderTatgetView()
	};
	m_finalRT.Clear(color);
	deviceContext->OMSetRenderTargets(1, finalrtv, m_finalRT.GetDepthStencilView());
	deviceContext->RSSetViewports(1, m_finalRT.GetViewPort());
	ID3D11ShaderResourceView* srv[] = {
		m_srv,
		m_avelumiRT.GetShaderResourceView()
	};
	deviceContext->VSSetShaderResources(0, 2, srv);
	deviceContext->PSSetShaderResources(0, 2, srv);
	m_postEffect.SetPS(&m_finalPS);
	m_postEffect.SetVS(&m_vs);
	m_postEffect.Draw();



	//レンダーターゲットをもとに戻す
	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDSV);
	deviceContext->RSSetViewports(*num, &buckUpViewPort);
	deviceContext->ClearRenderTargetView(buckUpRTV, color);
	deviceContext->ClearDepthStencilView(buckUpDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	ID3D11ShaderResourceView* copysrv[] = {
		m_finalRT.GetShaderResourceView()
	};
	deviceContext->VSSetShaderResources(0, 1, copysrv);
	deviceContext->PSSetShaderResources(0, 1, copysrv);
	m_postEffect.SetPS(&m_copyPS);
	m_postEffect.SetVS(&m_vs);
	m_postEffect.Draw();


}
