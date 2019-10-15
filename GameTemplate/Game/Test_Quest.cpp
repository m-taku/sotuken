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
	wchar_t na[256];
	fread(&na, sizeof(char) * count, 1, fp);
}
