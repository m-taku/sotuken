#include "stdafx.h"
#include "Test_Quest.h"
#include"Test_QuestBase.h"
#include<codecvt>

Test_Quest::Test_Quest()
{
}


Test_Quest::~Test_Quest()
{
}
bool Test_Quest::Init(const wchar_t* fileName)
{
	FILE* fp = _wfopen(fileName, L"rb");
	if (fp == nullptr)
	{
		return false;
	}
	int count = 0;
	fread(&count, sizeof(int), 1, fp);
	char* hoge = new char[count];
	fread(hoge, sizeof(char),count, fp);
	//std::string mane = hoge;
	//err = wcstombs_s(&kosuu, nja, 256, hoge, _TRUNCATE);
	sprintf_s(name[Test_QuestBase::Quest::titor],count, "%s", hoge);
	int mons = 0;
	fread(&mons, sizeof(int), 1, fp);
	MonsterNo = (Moster)mons;
	sprintf_s(name[Test_QuestBase::Quest::moster],256, "%s‚Ì“¢”°", JapanMosterName[MonsterNo]);
	int housyuu = 0;
	fread(&housyuu, sizeof(int), 1, fp);
	sprintf_s(name[Test_QuestBase::Quest::kane],256, "%d‚f", housyuu);
	int g_time = 0;
	fread(&g_time, sizeof(int), 1, fp);
	m_time = g_time;
	sprintf_s(name[Test_QuestBase::Quest::time], 256, "%d•ª", g_time);
	int daun = 0;
	fread(&daun, sizeof(int), 1, fp);
	sprintf_s(name[Test_QuestBase::Quest::doun],256, "%dƒ_ƒEƒ“", daun);

	m_Maxdoun = daun;
	
	count = 0;
	fread(&count, sizeof(int), 1, fp);
	auto hoge1 = new char[count];
	fread(hoge1, sizeof(char), count, fp);
	sprintf_s(name[Test_QuestBase::Quest::iraisya], 256, "%s", hoge1);


	count = 0;
	fread(&count, sizeof(int), 1, fp);
	auto hoge3 = new char[count];
	fread(hoge3, sizeof(char)  ,count, fp);

	sprintf_s(name[Test_QuestBase::Quest::bikou], 256, "%s", hoge3);
	fclose(fp);
	delete hoge3;
	delete hoge1;
	delete hoge;
	return true;
}
