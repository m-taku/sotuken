#include "stdafx.h"
#include "TestSave.h"
#include"Player.h"
#include <fstream>

TestSave::TestSave()
{
}


TestSave::~TestSave()
{
}
//bool TestSave::Seve()
//{
//	//�����ŃN���X���������A�Z�[������f�[�^�����
//	auto player = FindGO<Player>("player");
//	std::fstream fout;
//	fout.open("file.dadadadadada", std::ios::binary | std::ios::out);
//	fout.write((char *)player, sizeof(Player));
//	fout.close();
//	/*//�l�b�g�̃R�[�h
//	// �o�C�i���o�̓��[�h�ŊJ��
//	fstream file("./filename.dat", ios::binary | ios::out);
//	// �������ł��ǂ�
//	//file.open("./filename.dat", ios::binary | ios::out);
//	// �ǂݍ��ނȂ炱��
//	//file.open("./filename.dat", ios::binary | ios::in);
//
//	// ��������
//	file.write((char*)&n, sizeof(n));
//
//	// �ǂݍ���
//	//file.read((char*)&n, sizeof(n));
//	//cout << n << endl;
//
//	// ����
//	file.close();
//
//
//	*/
//	return true;
//
//}