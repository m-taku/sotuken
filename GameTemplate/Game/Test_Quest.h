#pragma once
#include"Test_QuestBase.h"
class Test_Quest :public Test_QuestBase
{
public:
	Test_Quest();
	~Test_Quest();
	bool Init(const wchar_t* fileName);

};

