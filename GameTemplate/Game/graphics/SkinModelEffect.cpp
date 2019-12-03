#include "stdafx.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/SkinModelShaderConst.h"

void __cdecl ModelEffect::Apply(ID3D11DeviceContext* deviceContext)
{

	switch (m_drawMode)
	{
	case enNormal: {
		//if (isSkining)
		//{
		//	m_vsShader.Load("Assets/shader/model.fx", "VSMainSkin", Shader::EnType::VS);
		//}
		//else
		//{
		//	m_vsShader.Load("Assets/shader/model.fx", "VSMain", Shader::EnType::VS);
		//}
		m_pVSShader = &m_vsShader;
		m_pPSShader = &m_psShader;
		//deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &m_albedoTex);
		break;
	}
	case enShadow: {
		//if (isSkining)
		//{
		//	m_vsShader.Load("Assets/shader/shadowmap.fx", "VSMainSkin", Shader::EnType::VS);
		//}
		//else
		//{
		//	m_vsShader.Load("Assets/shader/shadowmap.fx", "VSMain", Shader::EnType::VS);
		//}
		m_pVSShader = &m_vsShaderShadow;
		m_pPSShader = &m_psShaderShadow;
		//m_psShader.Load("Assets/shader/shadowmap.fx", "PSMain", Shader::EnType::PS);
		break;
	}
	case enInstancing: {
		//if (isSkining)
		//{
		//	m_vsShader.Load("Assets/shader/shadowmap.fx", "VSMainSkin", Shader::EnType::VS);
		//}
		//else
		//{
		//	m_vsShader.Load("Assets/shader/shadowmap.fx", "VSMain", Shader::EnType::VS);
		//}
		m_pVSShader = &m_vsShaderInstancing;
		m_pPSShader = &m_psShaderInstancing;
		//m_psShader.Load("Assets/shader/shadowmap.fx", "PSMain", Shader::EnType::PS);
		break;
	}
	case enInstancingShadow:{

		m_pVSShader = &m_vsShaderInstancingShadow;
		m_pPSShader = &m_psShaderShadowInstancing;
		break;
	}
	default: break;
	}
	deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &m_albedoTex);
	deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
	deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
}