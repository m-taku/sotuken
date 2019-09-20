#include "stdafx.h"
#include "ConstantBuffer.h"


ConstantBuffer::ConstantBuffer()
{
}


ConstantBuffer::~ConstantBuffer()
{
	Release();
}

bool ConstantBuffer::Create(const void * pInitData, int bufferSize)
{
	//ConstantBuffer�p�̃o�b�t�@��`���쐬����B
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//16�o�C�g�A���C�����g�ɐ؂肠����B
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	HRESULT hr;
	if (pInitData) {
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pInitData;
		hr = GraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, &InitData, &m_gpuBuffer);
	}
	else {
		hr = GraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_gpuBuffer);
	}
	if (FAILED(hr)) {
		return false;
	}
	return true;
}

void ConstantBuffer::Release()
{
	if (m_gpuBuffer != nullptr) {
		m_gpuBuffer->Release();
		m_gpuBuffer = nullptr;
	}
}
