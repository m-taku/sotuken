#include "stdafx.h"
#include "LightManager.h"

namespace smEngine
{
	LightManager::LightManager()
	{
	}


	LightManager::~LightManager()
	{
	}

	void LightManager::Init()
	{
		InitDirectionLightSB();
		InitPointLightSB();
		m_lightParamCB.Create(&m_lightParam, sizeof(m_lightParam));
	}

	void LightManager::AddLight(LightBase * light)
	{
		const std::type_info& typeInfo = typeid(*light);
		if (typeInfo == typeid(DirectionLight))
		{
			if (m_deirectionLights.size() >= MAX_DIRECTION_LIGHT)
			{
				/*ライト置きすぎ！！！！！！！！！！！！！*/
				abort();
			}
			auto findIt = std::find(m_deirectionLights.begin(), m_deirectionLights.end(), light);
			if (findIt == m_deirectionLights.end())
			{
				m_deirectionLights.push_back(reinterpret_cast<DirectionLight*>(light));
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
		m_lightParam.eyePos = smGameCamera().GetCameraPosition();
		m_lightParam.numDirectionLight = m_deirectionLights.size();
		m_lightParam.numPointLight = m_pointLights.size();
		int lightNo = 0;
		for (const auto& light : m_deirectionLights)
		{
			m_rawDirectionLights[lightNo] = light->GetRawData();
			lightNo++;
		}
		lightNo = 0;
		for (const auto&light : m_pointLights)
		{
			m_rawPointLights[lightNo] = light->GetRawData();
			lightNo++;
		}

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


