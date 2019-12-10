#include "stdafx.h"
#include "LightManager.h"

namespace smEngine
{
	LightManager::LightManager()
	{
	}


	LightManager::~LightManager()
	{
		for (auto direction : m_directionLights)
		{
			delete direction;
		}
	}

	void LightManager::Init()
	{
		InitDirectionLightSB();
		InitPointLightSB();
		m_lightParamCB.Create(&m_lightParam, sizeof(m_lightParam));
		
		m_shadowCollectRenderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_FLOAT);
		float color[] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowCollectRenderTarget.Clear(color);
		m_postEffect.Init();
		m_addShadowPS.Load("Assets/shader/shadowcollect.fx", "PSFinalMain", Shader::EnType::PS);
		m_copyPS.Load("Assets/shader/shadowcollect.fx", "PSCopyMain", Shader::EnType::PS);
		m_vs.Load("Assets/shader/shadowcollect.fx", "VSMain", Shader::EnType::VS);
		m_postEffect.SetVS(&m_vs);
		m_blur.InitOriginalResolusion(m_shadowCollectRenderTarget.GetShaderResourceView(), 25.0f);
	}

	void LightManager::AddLight(LightBase * light)
	{
		const std::type_info& typeInfo = typeid(*light);
		if (typeInfo == typeid(DirectionLight))
		{
			if (m_directionLights.size() >= MAX_DIRECTION_LIGHT)
			{
				/*ライト置きすぎ！！！！！！！！！！！！！*/
				abort();
			}
			auto findIt = std::find(m_directionLights.begin(), m_directionLights.end(), light);
			if (findIt == m_directionLights.end())
			{
				DirectionLight* Light = reinterpret_cast<DirectionLight*>(light);
				if (Light->IsEnableShadow())
				{
					Light->GetShadowMap()->Init();
					Light->GetShadowMap()->UpdateDirection(Light->GetDirection());
				}

				m_directionLights.push_back(Light);
			}
			else
			{
				/*登録済み！！！！！！！！！！！！！！！！*/
				abort();
			}
		}
		else if (typeInfo == typeid(PointLight))
		{
			if (m_pointLights.size() >= MAX_POINT_LIGHT)
			{
				/*ライト置きすぎ！！！！！！！！！！！！！*/
				abort();
			}
			auto findIt = std::find(m_pointLights.begin(), m_pointLights.end(), light);
			if (findIt == m_pointLights.end())
			{
				m_pointLights.push_back(reinterpret_cast<PointLight*>(light));
			}
			else
			{
				/*登録済み！！！！！！！！！！！！！！！！*/
				abort();
			}
		}


	}

	void LightManager::Update()
	{
		m_shadowMapArray.clear();
		m_shadowMapSRV = NULL;
		m_lightParam.eyePos = smGameCamera().GetCameraPosition();
		m_lightParam.numDirectionLight = m_directionLights.size();
		m_lightParam.numPointLight = m_pointLights.size();
		int lightNo = 0;
		for (const auto& light : m_directionLights)
		{
			m_rawDirectionLights[lightNo] = light->GetRawData();
			if (light->IsEnableShadow())
			{
				light->GetShadowMap()->UpdateDirection(light->GetDirection());

				m_shadowMapArray.push_back(light->GetShadowMap());
			}
			lightNo++;
		}
		lightNo = 0;
		for (const auto&light : m_pointLights)
		{
			m_rawPointLights[lightNo] = light->GetRawData();
			lightNo++;
		}


	}
	void LightManager::ShadowRender()
	{
		ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		for (const auto& shadowMap : m_shadowMapArray)
		{
			shadowMap->ShadowCasterDraw();
		}

		float color[] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowCollectRenderTarget.Clear(color);
		bool enableShadow = false;
		for (const auto& shadowMap : m_shadowMapArray)
		{
			shadowMap->DrawToShadowCollector();
			enableShadow = true;
		}

		if (enableShadow)
		{
			m_blur.Execute(0.3625f, 1.0f);

			ID3D11RenderTargetView* buckUpRTV = nullptr;
			ID3D11DepthStencilView* buckUpDepth = nullptr;
			D3D11_VIEWPORT buckUpViewPort;
			UINT numViewPort = 1;
			UINT* pnumViewPort = &numViewPort;
			deviceContext->OMGetRenderTargets(1, &buckUpRTV, &buckUpDepth);
			deviceContext->RSGetViewports(pnumViewPort, &buckUpViewPort);

			ID3D11RenderTargetView* copyrtv[] = {
				m_shadowCollectRenderTarget.GetRenderTatgetView()
			};
			deviceContext->OMSetRenderTargets(1, copyrtv, g_graphicsEngine->GetMainRenderTarget().GetDepthStencilView());
			deviceContext->RSSetViewports(1, g_graphicsEngine->GetMainRenderTarget().GetViewPort());

			ID3D11ShaderResourceView* copysrv[] = {
				m_blur.GetBlurSRV()
			};
			deviceContext->VSSetShaderResources(0, 1, copysrv);
			deviceContext->PSSetShaderResources(0, 1, copysrv);
			m_postEffect.SetPS(&m_copyPS);
			m_postEffect.Draw();

			deviceContext->OMSetRenderTargets(1, &buckUpRTV, buckUpDepth);
			deviceContext->RSSetViewports(*pnumViewPort, &buckUpViewPort);
		}
	}
	void LightManager::SendBuffer()
	{
		ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		deviceContext->UpdateSubresource(m_lightParamCB.GetBody(), 0, NULL, &m_lightParam, 0, 0);
		
		deviceContext->UpdateSubresource(m_directionLightSB.GetBody(), 0, NULL, m_rawDirectionLights, 0, 0);
		deviceContext->UpdateSubresource(m_pointLightSB.GetBody(), 0, NULL, m_rawPointLights, 0, 0);
		deviceContext->PSSetConstantBuffers(enLightParam_Slot_No, 1, &m_lightParamCB.GetBody());
		
		deviceContext->PSSetShaderResources(enDerectionLightSB_Slot_No, 1, &m_directionLightSB.GetSRV().GetBody());
		deviceContext->PSSetShaderResources(enPointLightSB_Slot_No, 1, &m_pointLightSB.GetSRV().GetBody());

	}
	void LightManager::InitDirectionLightSB()
	{
		int stride = sizeof(SDirectionLight);
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.ByteWidth = stride * MAX_DIRECTION_LIGHT;
		desc.StructureByteStride = stride;
		m_directionLightSB.Create(NULL, desc);
	}
	void LightManager::InitPointLightSB()
	{
		int stride = sizeof(SPointLight);
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.ByteWidth = stride * MAX_POINT_LIGHT;
		desc.StructureByteStride = stride;
		m_pointLightSB.Create(NULL, desc);
	}
}


