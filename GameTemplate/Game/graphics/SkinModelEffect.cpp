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
	case enCubeMap:
		m_pVSShader = &m_vsSkyShader;
		m_pPSShader = &m_psSkyShader;
		break;
	case enTree:
	{
		m_pVSShader = &m_vsShaderNature;
		m_pPSShader = &m_psTreeShader;
		deviceContext->RSSetState(g_graphicsEngine->GetRasterizerState());
		break;
	}
	case enTreeInstancing:
		m_pVSShader = &m_vsShaderInstancingNature;
		m_pPSShader = &m_psTreeShader;
		deviceContext->RSSetState(g_graphicsEngine->GetRasterizerState());
		break;
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
		m_pPSShader = &m_psShader;
		//m_psShader.Load("Assets/shader/shadowmap.fx", "PSMain", Shader::EnType::PS);
		break;
	}
	case enTreeShadow:
	{
		m_pVSShader = &m_vsShaderShadow;
		m_pPSShader = &m_psTreeShaderShadow; 
		break;
	}
	case enTreeShadowInstancing:
	{	
		m_pVSShader = &m_vsShaderInstancingShadow;;
		m_pPSShader = &m_psTreeShaderShadow;
		break;
	}
	case enInstancingShadow:{
		m_pVSShader = &m_vsShaderInstancingShadow;
		m_pPSShader = &m_psShaderShadow;
		break;
	}
	default: break;
	}
	//deviceContext->RSSetState(g_graphicsEngine->GetRasterizerState());

	deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &m_albedoTex);
	deviceContext->PSSetShaderResources(enSkinModelSRVReg_NomalTexture, 1, &m_normalTex);
	deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
	deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
}