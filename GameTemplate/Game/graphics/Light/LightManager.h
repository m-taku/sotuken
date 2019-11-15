#pragma once
namespace smEngine {
	class LightManager
	{
	public:
		LightManager();
		~LightManager();

		void Init();
		void AddLight(LightBase* light);
		void Update();

		static LightManager& Instance()
		{
			static LightManager instance;
			return instance;
		}

		void AddSkinModel(SkinModel* model)
		{
			m_skinModelArray.push_back(model);
		}

		void DeleteSkinModel(SkinModel* model)
		{
			m_skinModelArray.erase(
				std::remove(m_skinModelArray.begin(), m_skinModelArray.end(), model),
				m_skinModelArray.end()
			);
		}

		void ShadowRender();

		ID3D11ShaderResourceView* GetShadowSRV()
		{
			return m_shadowCollectRenderTarget.GetShaderResourceView();
		}

		RenderTarget& GetShadowRenderTarget()
		{
			return m_shadowCollectRenderTarget;
		}

		void SendBuffer();

		std::list<SkinModel*>& GetSkinModel()
		{
			return m_skinModelArray;
		}

		std::list<ShadowMap*>& GetShadowMapArray()
		{
			return m_shadowMapArray;
		}

	private:
		void InitDirectionLightSB();
		void InitPointLightSB();

	private:
		static const int MAX_DIRECTION_LIGHT = 8;						//!<ディレクションライトの最大数。
		static const int MAX_POINT_LIGHT = 1024;						//!<ポイントライトの最大数。

		struct SLightParam {
			CVector3 eyePos;
			int numDirectionLight;
			int numPointLight;
		};

		SLightParam m_lightParam;
		ConstantBuffer m_lightParamCB;

		SDirectionLight m_rawDirectionLights[MAX_DIRECTION_LIGHT];
		SPointLight m_rawPointLights[MAX_POINT_LIGHT];

		std::list<DirectionLight*> m_directionLights;
		std::list<PointLight*> m_pointLights;
		StructuredBuffer m_directionLightSB;
		StructuredBuffer m_pointLightSB;


		std::list<SkinModel*> m_skinModelArray;
		std::list <ShadowMap*> m_shadowMapArray;

		RenderTarget m_shadowCollectRenderTarget;
		PostEffect m_postEffect;
		GaussianBlur m_blur;
		void* m_shadowMapSRV = NULL;
	};

	static LightManager& smLightManager()
	{
		return LightManager::Instance();
	}
}


