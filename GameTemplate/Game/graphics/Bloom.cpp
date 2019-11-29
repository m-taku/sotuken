#include "stdafx.h"
#include "Bloom.h"


Bloom::Bloom()
{
}


Bloom::~Bloom()
{
	Release();
}

void Bloom::Init(ID3D11ShaderResourceView* srv)
{
	//参照カウンタを増やす
	srv->AddRef();
	m_srcTexture = srv;
	//元画像の解像度を取得
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* tex;
	srv->GetResource((ID3D11Resource**)&tex);
	tex->GetDesc(&desc);
	m_srcTexWidth = desc.Width;
	m_srcTexHeight = desc.Height;
	tex->Release();

	//通常のブレンドステートを作成
	ID3D11Device* device = g_graphicsEngine->GetD3DDevice();
	CD3D11_DEFAULT defaultState;
	CD3D11_BLEND_DESC disableBlendState(defaultState);

	disableBlendState.RenderTarget[0].BlendEnable = true;
	disableBlendState.RenderTarget[0].BlendOp = disableBlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	disableBlendState.RenderTarget[0].DestBlend = disableBlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	disableBlendState.RenderTarget[0].SrcBlend = disableBlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	device->CreateBlendState(&disableBlendState, &m_disableBlendState);
	//加算合成用のブレンディングステート作成
	CD3D11_BLEND_DESC finalBlendState(defaultState);

	finalBlendState.RenderTarget[0].BlendEnable = true;
	finalBlendState.RenderTarget[0].SrcBlend = finalBlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	finalBlendState.RenderTarget[0].DestBlend = finalBlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	device->CreateBlendState(&finalBlendState, &m_finalBlendState);
	//各種レンダーターゲットを作成
	float color[] = { 0.0f,0.0f,0.0f,0.0f };
	m_luminanceRT.Create(m_srcTexWidth, m_srcTexHeight, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_luminanceRT.Clear(color);
	float mixRTColor[] = { 0.0f,0.0f,0.0f,1.0f };
	m_mixBlurTexRT.Create(m_srcTexWidth, m_srcTexHeight, DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_mixBlurTexRT.Clear(mixRTColor);
	//ブラーの初期化
	m_blur[0].Init(m_luminanceRT.GetShaderResourceView());

	for (int i = 1; i < DOWN_SAMPLE_NUM; i++)
	{
		m_blur[i].Init(m_blur[i - 1].GetBlurSRV());
	}
	//レンダーの初期化
	m_postEffect.Init();
	//各種シェーダーのロード
	m_luminancePS.Load("Assets/shader/bloom.fx", "PSLuminance", Shader::EnType::PS);
	m_luminanceVS.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_mixBlurVS.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_mixBlurPS.Load("Assets/shader/bloom.fx", "PSMixBlurTex", Shader::EnType::PS);
	m_finalBlendVS.Load("Assets/shader/bloom.fx", "VSMain", Shader::EnType::VS);
	m_finalBlendPS.Load("Assets/shader/bloom.fx", "PSFinal", Shader::EnType::PS);
	//........ショキカ
	m_finalBlur.InitOriginalResolusion(m_mixBlurTexRT.GetShaderResourceView());
}

void Bloom::Draw()
{
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//レンダーターゲットのバックアップ
	ID3D11RenderTargetView* buckUpRTV;
	ID3D11DepthStencilView* buckUpDSV;
	D3D11_VIEWPORT buckUpViewPort;
	UINT numViewPort=1;
	UINT* num = &numViewPort;
	deviceContext->OMGetRenderTargets(1, &buckUpRTV, &buckUpDSV);
	deviceContext->RSGetViewports(num, &buckUpViewPort);
	//アルファブレンディングを無効にする
	float blendFactor[] = { 0.0f,0.0f,0.0f,0.0f };
	deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
	{//rtvって名前後で使いたいからスコープ付けてる
		//輝度を抽出
		float color[] = { 0.0f,0.0f,0.0f,0.0f };
		m_luminanceRT.Clear(color);
		ID3D11RenderTargetView* rtv[] = {
			m_luminanceRT.GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, rtv, m_luminanceRT.GetDepthStencilView());
		deviceContext->RSSetViewports(1, m_luminanceRT.GetViewPort());
		deviceContext->PSSetShaderResources(0, 1, &m_srcTexture);
		m_postEffect.SetPS(&m_luminancePS);
		m_postEffect.SetVS(&m_luminanceVS);
		m_postEffect.Draw();
	}
	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDSV);
	deviceContext->RSSetViewports(*num, &buckUpViewPort);
	//輝度テクスチャをボカス
	for (int i = 0; i < DOWN_SAMPLE_NUM; i++)
	{
		m_blur[i].Execute(2.0f);
	}
	//ぼかしたテクスチャを合成する
	{
		float mixRTColor[] = { 0.0f,0.0f,0.0f,1.0f };
		m_mixBlurTexRT.Clear(mixRTColor);
		ID3D11RenderTargetView* rtv[] = {
			m_mixBlurTexRT.GetRenderTatgetView()
		};
		deviceContext->OMSetRenderTargets(1, rtv, m_mixBlurTexRT.GetDepthStencilView());
		deviceContext->RSSetViewports(1, m_mixBlurTexRT.GetViewPort());
	}
	//
	for (int i = 0; i < DOWN_SAMPLE_NUM; i++)
	{
		ID3D11ShaderResourceView* srv[] = {
			m_blur[i].GetBlurSRV()
		};
		deviceContext->VSSetShaderResources(i, 1, srv);
		deviceContext->PSSetShaderResources(i, 1, srv);
	}
	m_postEffect.SetPS(&m_mixBlurPS);
	m_postEffect.SetVS(&m_mixBlurVS);
	m_postEffect.Draw();
	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDSV);
	deviceContext->RSSetViewports(*num, &buckUpViewPort);
	//合成したボケ画像をさらにぼかして品質を高める
	m_finalBlur.Execute(2.0f);
	//ボケ画像を元画像に加算合成
	deviceContext->OMSetBlendState(m_finalBlendState, blendFactor, 0xffffffff);
	g_graphicsEngine->ChangeMainRenderTarget(false);
	ID3D11ShaderResourceView* srv[] = {
			m_finalBlur.GetBlurSRV()
	};
	deviceContext->VSSetShaderResources(0, 1, srv);
	deviceContext->PSSetShaderResources(0, 1, srv);
	m_postEffect.SetVS(&m_finalBlendVS);
	m_postEffect.SetPS(&m_finalBlendPS);
	m_postEffect.Draw();
	deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
	//レンダーターゲットをもとに戻す
	deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDSV);
	deviceContext->RSSetViewports(*num, &buckUpViewPort);
}

void Bloom::Release()
{
	if (m_srcTexture != nullptr)
	{
		m_srcTexture->Release();
	}
	if (m_disableBlendState != nullptr)
	{
		m_disableBlendState->Release();
	}
	if (m_finalBlendState != nullptr)
	{
		m_finalBlendState->Release();
	}
}
