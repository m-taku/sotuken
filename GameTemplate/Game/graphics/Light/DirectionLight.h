#pragma once
class DirectionLight :public LightBase
{
public:
	DirectionLight();
	~DirectionLight();
	bool Start();
	void Update();
private:
};

