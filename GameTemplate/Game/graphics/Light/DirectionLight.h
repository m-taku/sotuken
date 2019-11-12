#pragma once
class DirectionLight :public LightBase
{
public:
	DirectionLight();
	~DirectionLight();
	bool Start();
	void Update();
	void SetDirection(const CVector3& direction)
	{
		m_light.direction = direction;
		m_light.direction.Normalize();
	}
	void SetColor(const CVector4& color)
	{
		m_light.color = color;
	}

	void SetEnableShadow(bool flag)
	{
		m_enableShadow = flag;
	}
	const CVector3& GetDirection() const
	{
		return m_light.direction;
	}
	const CVector4& GetColor() const
	{
		return m_light.color;
	}

	SDirectionLight& GetRawData()
	{
		return m_light;
	}

	bool IsEnableShadow()
	{
		return m_enableShadow;
	}
	
	ShadowMap* GetShadowMap()
	{
		return &m_shadowmap;
	}
private:
	SDirectionLight m_light;
	ShadowMap m_shadowmap;
	bool m_enableShadow = false;
};

