#pragma once
class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool State();
	void Update();
	void Draw();
private:
	CVector3 pos = { 0.0f,-500.0f,-300.0f };
	SkinModel m_model;
};

