#include "stdafx.h"
#include "Test_Quest.h"
#include"Test_QuestBase.h"


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
	wchar_t* hoge = new wchar_t[count];
	fread(hoge, sizeof(char)*count, 1, fp);

	swprintf_s(name[Test_QuestBase::Quest::titor], 256, L"%s", hoge);

	int mons = 0;
	fread(&mons, sizeof(int), 1, fp);
	auto monsname = monsuta[mons];
	swprintf_s(name[Test_QuestBase::Quest::monsu],256, L"%sÇÃì¢î∞", monsname);


	int housyuu = 0;
	fread(&housyuu, sizeof(int), 1, fp);
	swprintf_s(name[Test_QuestBase::Quest::kane],256, L"%dG", housyuu);

	int g_time = 0;
	fread(&g_time, sizeof(int), 1, fp);
	swprintf_s(name[Test_QuestBase::Quest::time],256,L"%dï™", g_time);


	int daun = 0;
	fread(&daun, sizeof(int), 1, fp);
	swprintf_s(name[Test_QuestBase::Quest::doun],256, L"%dÉ_ÉEÉì", daun);
	
	
	count = 0;
	fread(&count, sizeof(int), 1, fp);
	auto hoge1 = new wchar_t[count];
	fread(hoge1, sizeof(char) * count, 1, fp);
	swprintf_s(name[Test_QuestBase::Quest::iraisya], 256, L"àÀóäé“ÅF%s", hoge1);


	count = 0;
	fread(&count, sizeof(int), 1, fp);
	auto hoge3 = new wchar_t[count];
	fread(hoge3, sizeof(char) * count, 1, fp);

	swprintf_s(name[Test_QuestBase::Quest::bikou], 256, L"%s", hoge3);

	fclose(fp);
	return true;
}
