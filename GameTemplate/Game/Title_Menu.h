#pragma once
class Text_Box;
class Title_Menu : public IGameObject
{
public:
	Title_Menu();
	~Title_Menu();
	enum Game_Select
	{
		NewGame,
		LoadGame
	};
	bool Start();
	void Update();
	void Draw();
private:
	std::list<Text_Box*> m_text;
	Game_Select m_state = NewGame;
};

