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
std::string wide_to_utf8_cppapi(std::wstring const& src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(src);
}
std::wstring utf8_to_wide_cppapi(std::string const& src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(src);
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
//	std::string mane = hoge;
	//err = wcstombs_s(&kosuu, nja, 256, hoge, _TRUNCATE);
	sprintf_s(name[Test_QuestBase::Quest::titor], 256, "%s", hoge);

	int mons = 0;
	fread(&mons, sizeof(int), 1, fp);
	auto monsname = monsuta[mons];
	sprintf_s(name[Test_QuestBase::Quest::monsu],256, "%sÇÃì¢î∞", monsname);


	int housyuu = 0;
	fread(&housyuu, sizeof(int), 1, fp);
	sprintf_s(name[Test_QuestBase::Quest::kane],256, "%dG", housyuu);

	int g_time = 0;
	fread(&g_time, sizeof(int), 1, fp);
	sprintf_s(name[Test_QuestBase::Quest::time],256,"%dï™", g_time);


	int daun = 0;
	fread(&daun, sizeof(int), 1, fp);
	sprintf_s(name[Test_QuestBase::Quest::doun],256, "%dÉ_ÉEÉì", daun);
	
	
	count = 0;
	fread(&count, sizeof(int), 1, fp);
	auto hoge1 = new wchar_t[count];
	fread(hoge1, sizeof(char) * count, 1, fp);
	sprintf_s(name[Test_QuestBase::Quest::iraisya], 256, "àÀóäé“ÅF%s", hoge1);


	count = 0;
	fread(&count, sizeof(int), 1, fp);
	auto hoge3 = new wchar_t[count];
	fread(hoge3, sizeof(char) * count, 1, fp);

	sprintf_s(name[Test_QuestBase::Quest::bikou], 256, "%s", hoge3);

	fclose(fp);
	return true;
}
