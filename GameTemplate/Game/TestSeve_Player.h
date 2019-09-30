#pragma once
#include"TestSave.h"
class TestSeve_Player : public TestSave
{
public:
	TestSeve_Player(int No);
	~TestSeve_Player();
	bool Seve()override;
	void Load() override;
private:
	wchar_t failpas[256];
};

