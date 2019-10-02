#pragma once
class Text_Box;
class SaveData_Select : public IGameObject
{
public:
	SaveData_Select();
	~SaveData_Select();
	enum SaveNo
	{
		No1,
		No2,
		No3
	};
	bool Start();
	void Update();
	void Draw();
	void k();
private:
	SaveNo No = No1;
	std::list<Text_Box*> m_text;
};

