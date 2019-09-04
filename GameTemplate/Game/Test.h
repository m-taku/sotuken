#pragma once
class Test : public IGameObject
{
public:
	Test();
	~Test();
	bool State();
	void Update();
	void Draw();
private:
	int No = 0;
	CVector3 pos = {100.0f,0.0f,0.0f};
	SkinModel m_model;

};

