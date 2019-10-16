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
		m_lightParamCB.Create(&m_lightParam, sizeof(m_lightParam));

		int stride = sizeof(SDirectionLight);
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.ByteWidth = stride * MAX_DIRECTION_LIGHT;
		desc.StructureByteStride = stride;
		m_directionLightSB.Create(NULL, desc);
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
	}

	void LightManager::Update()
	{
		m_lightParam.eyePos = smGameCamera().GetCameraPosition();
		m_lightParam.numDirectionLight = m_deirectionLights.size();
		int lightNo = 0;
		for (const auto& light : m_deirectionLights)
		{
			m_rawDirectionLights[lightNo] = light->GetRawData();
			lightNo++;
		}

		ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		deviceContext->UpdateSubresource(m_lightParamCB.GetBody(), 0, NULL, &m_lightParam, 0, 0);
		deviceContext->UpdateSubresource(m_directionLightSB.GetBody(), 0, NULL, m_rawDirectionLights, 0, 0);
		deviceContext->PSSetConstantBuffers(enLightParam_Slot_No, 1, &m_lightParamCB.GetBody());
		deviceContext->PSSetShaderResources(enDerectionLightSB_Slot_No, 1, &m_directionLightSB.GetSRV().GetBody());
	}
}


