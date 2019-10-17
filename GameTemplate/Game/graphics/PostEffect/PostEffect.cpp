#include "stdafx.h"
#include "graphics/PostEffect/PostEffect.h"

namespace {
	struct SSimpleVertex {
		CVector4 pos;
		CVector2 tex;
	};
}

PostEffect::PostEffect()
{
	m_viewMatrix.MakeLookAt(
		{ 0.0f,0.0f,-5.0f },
		CVector3::Zero(),
		CVector3::Up()
	);
	m_projMatrix.MakeOrthoProjectionMatrix(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		1.0f,
		10.0f
	);
}


PostEffect::~PostEffect()
{
}

void PostEffect::Init()
{
	m_ps.Load("Assets/shader/posteffect.fx", "PSMain", Shader::EnType::PS);
	m_vs.Load("Assets/shader/posteffect.fx", "VSMain", Shader::EnType::VS);

	SSimpleVertex vertices[] =
	{
			{
				CVector4(-1.0f,-1.0f,0.0f,1.0f),
				CVector2(0.0f,1.0f),
			},
			{
				CVector4(1.0f,-1.0f,0.0f,1.0f),
				CVector2(1.0f,1.0f),
			},
			{
				CVector4(-1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}
	};


	short indices[] = { 0,1,2,3 };

	m_primitive.Create(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		4,
		sizeof(SSimpleVertex),
		vertices,
		4,
		IndexBuffer::enIndexType_16,
		indices
	);

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}

void PostEffect::Draw()
{
	ID3D11DeviceContext* DeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	DeviceContext->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), NULL, 0);
	DeviceContext->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), NULL, 0);
	DeviceContext->IASetInputLayout(m_vs.GetInputLayout());
	DeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	m_primitive.Draw(*DeviceContext);
}

void PostEffect::Release()
{
	if (m_samplerState != nullptr)
	{
		m_samplerState->Release();
	}
}
