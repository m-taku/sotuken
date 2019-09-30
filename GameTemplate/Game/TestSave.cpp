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
//	//ここでクラスを検索し、セーヴするデータを作る
//	auto player = FindGO<Player>("player");
//	std::fstream fout;
//	fout.open("file.dadadadadada", std::ios::binary | std::ios::out);
//	fout.write((char *)player, sizeof(Player));
//	fout.close();
//	/*//ネットのコード
//	// バイナリ出力モードで開く
//	fstream file("./filename.dat", ios::binary | ios::out);
//	// こっちでも良し
//	//file.open("./filename.dat", ios::binary | ios::out);
//	// 読み込むならこう
//	//file.open("./filename.dat", ios::binary | ios::in);
//
//	// 書き込む
//	file.write((char*)&n, sizeof(n));
//
//	// 読み込む
//	//file.read((char*)&n, sizeof(n));
//	//cout << n << endl;
//
//	// 閉じる
//	file.close();
//
//
//	*/
//	return true;
//
//}