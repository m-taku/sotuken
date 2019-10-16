#pragma once

class PointLight :public LightBase
{
public:
	PointLight();
	~PointLight();
	bool Start();
	void Update();

	void SetPosition(const CVector3& position)
	{
		m_light.position = position;
	}
	void SetColor(const CVector4& color)
	{
		m_light.color = color;
	}
	void SetAttn(const CVector4& attn)
	{
		m_light.attn = attn;
	}
	const CVector3& GetPosition() const
	{
		return m_light.position;
	}
	const CVector4& GetColor() const
	{
		return m_light.color;
	}

	SPointLight& GetRawData()
	{
		return m_light;
	}
private:
	SPointLight m_light;
};

