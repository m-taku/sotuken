#include "stdafx.h"
#include "ShaderResourceView.h"

ShaderResourceView::ShaderResourceView()
{
}


ShaderResourceView::~ShaderResourceView()
{
	Release();
}
bool ShaderResourceView::Create(StructuredBuffer& structuredBuffer)
{

	Release();
	ID3D11Buffer* pBuf = structuredBuffer.GetBody();
	if (pBuf != nullptr)
	{
		D3D11_BUFFER_DESC descBuf;
		ZeroMemory(&descBuf, sizeof(descBuf));
		pBuf->GetDesc(&descBuf);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		desc.BufferEx.NumElements = 0;

		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

		HRESULT hr = g_graphicsEngine->GetD3DDevice()->CreateShaderResourceView(pBuf, &desc, &m_srv);
		if (FAILED(hr))
		{
			return false;
		}
	}
	m_isValid = true;
	return true;
}

bool ShaderResourceView::Create(ID3D11Texture2D * texture)
{
	Release();
	if (texture != nullptr)
	{
		D3D11_TEXTURE2D_DESC texDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = texDesc.MipLevels;

		HRESULT hr = g_graphicsEngine->GetD3DDevice()->CreateShaderResourceView(texture, &SRVDesc, &m_srv);
		//ì¬‚ÉŽ¸”s‚µ‚½‚çfalse‚ð•Ô‚·
		if (FAILED(hr))
		{
			return false;
		}
		m_isValid = true;
	}
	return true;
}

bool ShaderResourceView::CreateFromDDSTextureFromFile(const wchar_t * fileName)
{
	Release();
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(),
		fileName,
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_srv
	);
	if (FAILED(hr))
	{
		return false;
	}
	m_isValid = true;
	return true;
}

void ShaderResourceView::Release()
{
	if (m_srv != nullptr) {
		m_srv->Release();
		m_srv = nullptr;
	}
	m_isValid = false;
}
