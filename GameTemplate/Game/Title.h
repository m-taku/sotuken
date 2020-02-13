#pragma once
class Text_Box;
class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Draw();
	void TextureDraw();
private:
	//ShaderResourceView m_data;
	//Sprite m_sprite;
	CVector3 pos = { 0.0f,-500.0f,-300.0f };
	Sprite m_sprite;
	ShaderResourceView m_srv;
	Text_Box* text = nullptr;
	CSoundSource* m_pSoundSource = nullptr;
};

