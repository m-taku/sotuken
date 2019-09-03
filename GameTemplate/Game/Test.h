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
	CVector3 pos = CVector3::Zero();
	SkinModel m_model;

};

