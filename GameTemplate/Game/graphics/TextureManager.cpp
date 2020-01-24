#include "stdafx.h"
#include "TextureManager.h"


//TextureManager�̎��́B
TextureManager g_TextureManager;
TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	Release();
}
ID3D11ShaderResourceView* TextureManager::Load(const wchar_t* filePath)
{
	ID3D11ShaderResourceView* retModel = nullptr;
	//�}�b�v�ɓo�^����Ă��邩���ׂ�B
	auto it = m_directX.find(filePath);
	if (it == m_directX.end()) {
		ID3D11ShaderResourceView* texSRV;
		HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
			g_graphicsEngine->GetD3DDevice(),
			filePath,
			0,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_SHADER_RESOURCE,
			0,
			0,
			false,
			nullptr,
			&texSRV
		);
		if (FAILED(hr))
		{
			DirectX::CreateDDSTextureFromFileEx(
				g_graphicsEngine->GetD3DDevice(),
				L"Assets/sprite/nomal.dds",
				0,
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_SHADER_RESOURCE,
				0,
				0,
				false,
				nullptr,
				&texSRV
			);
		}
		retModel = texSRV;
		//�V�K�Ȃ̂Ń}�b�v�ɓo�^����B
		m_directX.insert({ filePath, texSRV });
	}
	else {
		//�o�^����Ă���̂ŁA�ǂݍ��ݍς݂̃f�[�^���擾�B
		retModel = it->second;
	}
	return retModel;
}
void TextureManager::Release()
{
	for (auto ma : m_directX)
	{
		ma.second->Release();
	}
	m_directX.clear();
}