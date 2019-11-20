#include "stdafx.h"
#include "GameFont.h"


GameFont::GameFont()
{
	//m_SpriteBatch = new DirectX::SpriteBatch(m_pd3dDeviceContext);
	//m_SpriteFont = new DirectX::SpriteFont(m_pd3dDevice, L"Assets/font/floay.spritefont");
	m_spriteBatch = new DirectX::SpriteBatch(g_graphicsEngine->GetD3DDeviceContext());
	m_spriteFont = new DirectX::SpriteFont(g_graphicsEngine->GetD3DDevice(), L"Assets/font/floay.spritefont");
//	auto& ge = g_graphicsEngine;
	m_scaleMat.MakeScaling(
		{
			FRAME_BUFFER_W / static_cast<float>(FRAME_BUFFER_W),
			FRAME_BUFFER_H / static_cast<float>(FRAME_BUFFER_H),
			1.0f
		}
	);
}


GameFont::~GameFont()
{
	delete m_spriteFont; 
	delete m_spriteBatch;
}
void GameFont::BeginDraw()
{
	D3D11_BLEND_DESC BLEND_DETE;
	BLEND_DETE.AlphaToCoverageEnable = false;
	BLEND_DETE.IndependentBlendEnable = false;
	BLEND_DETE.RenderTarget[0].BlendEnable = 1;
	BLEND_DETE.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BLEND_DETE.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BLEND_DETE.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BLEND_DETE.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	BLEND_DETE.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	BLEND_DETE.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BLEND_DETE.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	ID3D11BlendState* BlendState;
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ID3D11Device* d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBlendState(&BLEND_DETE, &BlendState);
	d3dDeviceContext->OMSetBlendState(BlendState, nullptr, 0xFFFFFFFF);
	m_spriteBatch->Begin(
		DirectX::SpriteSortMode_Deferred,
		BlendState,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		m_scaleMat
	);


}
void GameFont::EndDraw()
{
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ID3D11Device* d3dDevice = g_graphicsEngine->GetD3DDevice();
	m_spriteBatch->End();
	////レンダリングステートを戻す。
	{
		//ブレンド設定
		D3D11_BLEND_DESC BLEND_DETE;
		BLEND_DETE.AlphaToCoverageEnable = false;
		BLEND_DETE.IndependentBlendEnable = false;
		BLEND_DETE.RenderTarget[0].BlendEnable = 1;
		BLEND_DETE.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BLEND_DETE.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BLEND_DETE.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BLEND_DETE.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		BLEND_DETE.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		BLEND_DETE.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BLEND_DETE.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState* BlendState;
		d3dDevice->CreateBlendState(&BLEND_DETE, &BlendState);
		d3dDeviceContext->OMSetBlendState(BlendState, nullptr, 0xFFFFFFFF);

		MemoryBarrier();
	}
	d3dDeviceContext->RSSetState(g_graphicsEngine->GetRasterizerState());

	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = true;
		desc.DepthWriteMask = false ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		desc.StencilEnable = false;
		desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		desc.BackFace = desc.FrontFace;

		ID3D11DepthStencilState* depthStencilState;
		d3dDevice->CreateDepthStencilState(&desc, &depthStencilState);
		d3dDeviceContext->OMSetDepthStencilState(depthStencilState, 0);
		MemoryBarrier();

	}
}
void GameFont::Draw(
	wchar_t const* text,
	const CVector2& position,
	const CVector4& color,
	float rotation,
	float scale,
	CVector2 pivot)
{

	pivot.y = 1.0f - pivot.y;
	DirectX::XMFLOAT2 tkFloat2Zero(0, 0);
	//座標系をスプライトと合わせる。
	CVector2 pos = position;
	float frameBufferHalfWidth = FRAME_BUFFER_W * 0.5f;
	float frameBufferHalfHeight = FRAME_BUFFER_H * 0.5f;
	pos.x += frameBufferHalfWidth;
	pos.y = -pos.y + frameBufferHalfHeight;
	m_spriteFont->DrawString(
		m_spriteBatch,
		text,
		pos.vec,
		color,
		rotation,
		DirectX::XMFLOAT2(pivot.x, pivot.y),
		scale*5.0f
	);
}