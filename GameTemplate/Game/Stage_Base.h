#pragma once
class Stage_Base:public IGameObject
{
public:
	Stage_Base();
	~Stage_Base();
	bool Start()
	{
		return true;
	}
	void Update()
	{

	}
	void Draw()
	{

	}
private:
	SkinModel m_l;
};

