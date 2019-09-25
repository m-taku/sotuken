#pragma once
class Text_Box;
class Title_Menu : public IGameObject
{
public:
	Title_Menu();
	~Title_Menu();
	bool Start();
	void Update();
	void Draw();
private:
	std::vector<Text_Box*> m_text;
};

