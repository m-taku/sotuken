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
	private:
		void InitDirectionLightSB();
		void InitPointLightSB();

	private:
		static const int MAX_DIRECTION_LIGHT = 8;						//!<�f�B���N�V�������C�g�̍ő吔�B
		static const int MAX_POINT_LIGHT = 1024;						//!<�|�C���g���C�g�̍ő吔�B

		struct SLightParam {
			CVector3 eyePos;
			int numDirectionLight;
			int numPointLight;
		};

		SLightParam m_lightParam;
		ConstantBuffer m_lightParamCB;

		SDirectionLight m_rawDirectionLights[MAX_DIRECTION_LIGHT];
		SPointLight m_rawPointLights[MAX_POINT_LIGHT];

		std::list<DirectionLight*> m_deirectionLights;
		std::list<PointLight*> m_pointLights;
		StructuredBuffer m_directionLightSB;
		StructuredBuffer m_pointLightSB;
	};

	static LightManager& smLightManager()
	{
		return LightManager::Instance();
	}
}


