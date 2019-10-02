#include "stdafx.h"
#include "TestSeve_Player.h"
#include <fstream>
#include"Test_PlayerData.h"


TestSeve_Player::TestSeve_Player(int No)
{
	swprintf_s(failpas, L"SaveData/Data%d/Palyer_Data_Test.smd",No);
	data = new Palyer_Data_Test;
	Load();
}

TestSeve_Player::~TestSeve_Player()
{
}
bool TestSeve_Player::Seve()
{
	std::fstream fout;	
	fout.open(failpas, std::ios::binary | std::ios::out);
	auto na = (Palyer_Data_Test*)data;
	na->hp += 10;
	fout.write((char*)na, sizeof(Palyer_Data_Test));
	fout.close();
	return true;
}
void TestSeve_Player::Load()
{
	std::fstream fout;
	fout.open(failpas, std::ios::binary | std::ios::in);
	auto  mamma = new Palyer_Data_Test;
	fout.read((char*)data, sizeof(Palyer_Data_Test));
	fout.close();
}