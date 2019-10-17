#pragma once
class Test_Quest;
class Test_GuestManager : public IGameObject
{
public:
	Test_GuestManager();
	~Test_GuestManager();
	bool Start();
	void Update();
private:
	int m_No = 0;
	std::vector<Test_Quest*> m_Quest;
};

