#include "stdafx.h"
#include "Test_GuestManager.h"
#include"Test_Quest.h"


Test_GuestManager::Test_GuestManager()
{
	int i = 0;
}


Test_GuestManager::~Test_GuestManager()
{
}
bool Test_GuestManager::Start()
{
	auto ka = NewGO<Test_Quest>(0);
	ka->Init(L"Assets/TestQeste/save0.smqd");
	//auto ka1 = NewGO<Test_Quest>(0);
	//ka1->Init(L"Assets/TestQeste/save1.smqd");

	//auto ka2 = NewGO<Test_Quest>(0);
	//ka1->Init(L"Assets/TestQeste/save2.smqd");

	//auto ka3 = NewGO<Test_Quest>(0);
	//ka1->Init(L"Assets/TestQeste/save3.smqd");

	//auto ka4 = NewGO<Test_Quest>(0);
	//ka1->Init(L"Assets/TestQeste/save4.smqd");
	return true;
}