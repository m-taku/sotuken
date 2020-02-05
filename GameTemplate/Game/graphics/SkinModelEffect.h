#pragma once

#include "graphics/Shader.h"
#include "TextureManager.h"

/*!
*@brief	モデルエフェクト。
*/
class ModelEffect : public DirectX::IEffect {
protected:
	std::wstring m_materialName;	//!<マテリアル名。
	Shader* m_pVSShader = nullptr;
	Shader* m_pPSShader = nullptr;
	Shader m_vsShader;
	Shader m_vsSkyShader;
	Shader m_vsShaderInstancing;
	Shader m_vsShaderShadow;
	Shader m_vsShaderInstancingShadow;


	Shader m_psShader;
	Shader m_psSkyShader;
	Shader m_psTreeShader;
	Shader m_psShaderShadow;
	Shader m_psTreeShaderShadow;

	bool isSkining;
	ID3D11ShaderResourceView* m_albedoTex = nullptr;
	bool isTextureManager = false;
	ID3D11ShaderResourceView* m_normalTex = nullptr;
	EnDrawMode m_drawMode = enNormal;

public:
	ModelEffect()
	{
		
		m_psShader.Load("Assets/shader/model.fx", "PSMain", Shader::EnType::PS);
		m_psSkyShader.Load("Assets/shader/skycube.fx", "PSCubeMain", Shader::EnType::PS);
		m_psShaderShadow.Load("Assets/shader/shadowmap.fx", "PSMain", Shader::EnType::PS);
		m_psTreeShader.Load("Assets/shader/model.fx", "PSTreeMain", Shader::EnType::PS);
		m_psTreeShaderShadow.Load("Assets/shader/shadowmap.fx", "PSTreeMain", Shader::EnType::PS);
		m_pPSShader = &m_psShader;
	}
	virtual ~ModelEffect()
	{
		if (!isTextureManager) {
			if (m_albedoTex) {
				m_albedoTex->Release();
			}
		}
	}



	void __cdecl Apply(ID3D11DeviceContext* deviceContext) override;

	void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override
	{
		*pShaderByteCode = m_vsShader.GetByteCode();
		*pByteCodeLength = m_vsShader.GetByteCodeSize();
	}
	void SetAlbedoTexture(ID3D11ShaderResourceView* tex,bool furag)
	{
		m_albedoTex = tex;
		isTextureManager = furag;
	}
	void SetNormalTexture(ID3D11ShaderResourceView* tex)
	{
		m_normalTex = tex;
	}
	void SetMatrialName(const wchar_t* matName)
	{
		m_materialName = matName;
	}

	bool EqualMaterialName(const wchar_t* name) const
	{
		return wcscmp(name, m_materialName.c_str()) == 0;
	}

	void SetDrawMode(EnDrawMode mode)
	{
		m_drawMode = mode;
	}
};
/*!
*@brief
*  スキンなしモデルエフェクト。
*/
class NonSkinModelEffect : public ModelEffect {
public:
	NonSkinModelEffect()
	{
		m_vsShader.Load("Assets/shader/model.fx", "VSMain", Shader::EnType::VS);
		m_vsSkyShader.Load("Assets/shader/skycube.fx", "VSMain", Shader::EnType::VS);
		m_vsShaderInstancing.Load("Assets/shader/model.fx", "VSMainInstancing", Shader::EnType::VS);
		m_vsShaderShadow.Load("Assets/shader/shadowmap.fx", "VSMain", Shader::EnType::VS);
		m_vsShaderInstancingShadow.Load("Assets/shader/shadowmap.fx", "VSMainInstancing", Shader::EnType::VS);
		m_pVSShader = &m_vsShader;
		isSkining = false;
	}
};
/*!
*@brief
*  スキンモデルエフェクト。
*/
class SkinModelEffect : public ModelEffect {
public:
	SkinModelEffect()
	{
		/*
		wchar_t hoge[256];
		GetCurrentDirectoryW(256, hoge);*/
		m_vsShader.Load("Assets/shader/model.fx", "VSMainSkin", Shader::EnType::VS);
		m_vsSkyShader.Load("Assets/shader/skycube.fx", "VSMain", Shader::EnType::VS);
		m_vsShaderInstancing.Load("Assets/shader/model.fx", "VSMainSkinInstancing", Shader::EnType::VS);
		m_vsShaderShadow.Load("Assets/shader/shadowmap.fx", "VSMainSkin", Shader::EnType::VS);
		m_vsShaderInstancingShadow.Load("Assets/shader/shadowmap.fx", "VSMainSkinInstancing", Shader::EnType::VS);
		m_pVSShader = &m_vsShader;
		isSkining = true;
	}
};

/*!
*@brief
*  エフェクトファクトリ。
*/
class SkinModelEffectFactory : public DirectX::EffectFactory {
public:
	SkinModelEffectFactory(ID3D11Device* device) :
		EffectFactory(device) {}
	std::shared_ptr<DirectX::IEffect> __cdecl CreateEffect(const EffectInfo& info, ID3D11DeviceContext* deviceContext)override
	{
		std::shared_ptr<ModelEffect> effect;
		if (info.enableSkinning) {
			//スキニングあり。
			effect = std::make_shared<SkinModelEffect>();
		}
		else {
			//スキニングなし。
			effect = std::make_shared<NonSkinModelEffect>();
		}
		effect->SetMatrialName(info.name);
		if (info.diffuseTexture && *info.diffuseTexture)
		{
			wchar_t moveFilePath[256];
			ID3D11ShaderResourceView* texSRV;
			auto ma = wcsstr(info.diffuseTexture, L"T_");
			if (ma != NULL) {
				wchar_t name[256];
				auto num = wcslen(ma);
				wcscpy(name, ma);
				name[num - 11] = NULL;
				swprintf_s(moveFilePath, L"Assets/sprite/T_%sBC.dds", &name[3]);
				texSRV = g_TextureManager.Load(moveFilePath);
				effect->SetAlbedoTexture(texSRV,true);
				swprintf_s(moveFilePath, L"Assets/sprite/T_%sN.dds", &name[3]);
				texSRV = g_TextureManager.Load(moveFilePath);
				effect->SetNormalTexture(texSRV);
			}
			else
			{
				DirectX::EffectFactory::CreateTexture(info.diffuseTexture, deviceContext, &texSRV);
				effect->SetAlbedoTexture(texSRV,false);
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
				effect->SetNormalTexture(texSRV);
			}
		}
		return effect;
	}

	void __cdecl CreateTexture(const wchar_t* name, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView** textureView) override
	{
		return DirectX::EffectFactory::CreateTexture(name, deviceContext, textureView);
	}
};
