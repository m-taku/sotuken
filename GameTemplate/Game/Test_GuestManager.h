#pragma once
#include"Test_Quest.h"
class Test_GuestManager : public IGameObject
{
public:
	Test_GuestManager();
	~Test_GuestManager();
	bool Start();
	void Update();
	void Printkami()
	{
		m_isActive = true;
		m_Quest[0]->ChangePaper();
	}
	void Printkami1()
	{
		m_isActive =false;
		m_Quest[m_No]->ChangePaper();
	}
private:
	int m_No = 0;
	std::vector<Test_Quest*> m_Quest;
};

