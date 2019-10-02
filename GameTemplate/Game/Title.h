#pragma once
class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Draw();
private:
	//ShaderResourceView m_data;
	//Sprite m_sprite;
	CVector3 pos = { 0.0f,-500.0f,-300.0f };
	SkinModel m_model;
};

