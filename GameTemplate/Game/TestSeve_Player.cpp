#include "stdafx.h"
#include "TestSeve_Player.h"
#include <fstream>
#include"Test_PlayerData.h"


TestSeve_Player::TestSeve_Player(int No)
{
	swprintf_s(failpas, L"SaveData/Data%d/Palyer_Data_Test.smd",No);
	data = new PalyerData;
	Load();
}

TestSeve_Player::~TestSeve_Player()
{
}
bool TestSeve_Player::Seve()
{
	std::fstream fout;	
	fout.open(failpas, std::ios::binary | std::ios::out);
	auto na = (PalyerData*)data;
	fout.write((char*)na, sizeof(PalyerData));
	fout.close();
	return true;
}
void TestSeve_Player::Load()
{
	std::fstream fout;
	fout.open(failpas, std::ios::binary | std::ios::in);
	fout.read((char*)data, sizeof(PalyerData));
	fout.close();
}