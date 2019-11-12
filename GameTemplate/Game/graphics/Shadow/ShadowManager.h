#pragma once
class ShadowManager
{
public:
	ShadowManager();
	~ShadowManager();

	void AddShadow(ShadowMap* shadowMap)
	{
		bool added = false;
		for (const auto& shadow : m_shadowMapArray)
		{
			if (shadow == shadowMap)
			{
				added = true;
			}
		}
		if (added)
		{
			m_shadowMapArray.push_back(shadowMap);
		}
	}
private:
	std::vector<ShadowMap*> m_shadowMapArray;
};

