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
	}
	void SetColor(const CVector4& color)
	{
		m_light.color = color;
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

private:
	SDirectionLight m_light;
};

