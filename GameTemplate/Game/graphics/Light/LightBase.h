#pragma once
class LightBase:Noncopyable
{
public:
	LightBase();
	~LightBase();
	virtual bool Start() { return true; }
	virtual void Update() {}
private:
	bool enableShadow = false;
};

