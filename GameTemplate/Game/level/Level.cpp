#include "stdafx.h"
#include "Level.h"
#include "MapChip.h"

using namespace std;

Level::Level()
{
}
Level::~Level()
{
}

void Level::Init(const wchar_t* levelDataFilePath, Level::HookWhenBuildObjectFunc hookFunc)
{
	//スケルトンをロードする。
	Skeleton skeleton;
	skeleton.Load(levelDataFilePath);
	//構築構築。
	//0番目はルートオブジェクトなので飛ばす。
	for (auto i = 1; i < skeleton.GetNumBones(); i++) {
		//骨を取得。
		auto bone = skeleton.GetBone(i);
		if (bone->GetParentId() == 0) {	//親がルートの場合だけマップチップを生成する。
			LevelObjectData objData;
			CVector3 scale;
			bone->CalcWorldTRS(objData.position, objData.rotation, scale);
			//3dsMaxとは軸が違うので、補正を入れる。
			auto t = objData.position.y;
			objData.position.y = objData.position.z;
			objData.position.z = -t;

			t = objData.rotation.y;
			objData.rotation.y = objData.rotation.z;
			objData.rotation.z = -t;
			objData.name = bone->GetName();
			bool isHook = false;
			if (hookFunc != nullptr) {
				//hook関数が指定されているのでhook関数を呼び出す。
				isHook = hookFunc(objData);
			}
			if (isHook == false) {
				//フックされなかったので、マップチップを作成する。
				auto mapChip = std::make_unique<MapChip>(objData);
				m_mapChipArray.push_back(std::move(mapChip));
			}
		}
	}
}
void Level::UEInit(const wchar_t* levelDataFilePath, Level::HookWhenBuildObjectFunc hookFunc)
{
	//tksファイルをオープン。
	FILE* fp = _wfopen(levelDataFilePath, L"rb");
	if (fp == nullptr) {
		return;
	}
	//骨の数を取得。
	int numBone = 0;
	//tksファイルの先頭から、4バイト読み込む。
	//先頭４バイトに骨の数のデータが入っている。
	fread(&numBone, sizeof(numBone), 1, fp);
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		LevelObjectData objData;
		int namelen;
		fread(&namelen, sizeof(int), 1, fp);
		//骨の名前を読み込めるだけのメモリを確保する。
		char* name = new char[namelen + 1];
		//骨の名前を読み込み。+1は終端文字列を読み込むため
		fread(name, namelen + 1, 1, fp);

		size_t kosuu;
		setlocale(LC_ALL, "japanese");
		wchar_t boneName[256];
		mbstowcs(boneName, name, 256);
		objData.name = boneName;
		//回転情報を読み込む
		fread(&objData.rotation, sizeof(CQuaternion), 1, fp);
		//objData.rotation = CQuaternion::Identity();
		//移動情報を読み込む
		CVector4 pos = CVector4::White();
		fread(&pos, sizeof(CVector4), 1, fp);
		objData.position.x = pos.x;
		objData.position.y = pos.y;
		objData.position.z = pos.z;
		//拡大情報を読み込む
		CVector4 scale = CVector4::White();
		fread(&scale, sizeof(CVector4), 1, fp);
		objData.scale.x = scale.x;
		objData.scale.y = scale.y;
		objData.scale.z = scale.z;
		auto t = objData.position.y;
		objData.position.x = objData.position.x;
		objData.position.y = objData.position.z;
		objData.position.z = t;
		auto ty = objData.rotation.y;
		auto tx = objData.rotation.x;
		auto tz = objData.rotation.z;
		objData.rotation.x = -tx;
		objData.rotation.y = -tz;
		objData.rotation.z = -ty;
		delete[] name;
		bool isHook = false;
		if (hookFunc != nullptr) {
			//hook関数が指定されているのでhook関数を呼び出す。
			isHook = hookFunc(objData);
		}
		if (isHook == false) {
			//フックされなかったので、マップチップを作成する。
			auto mapChip = std::make_unique<MapChip>(objData);
			m_mapChipArray.push_back(std::move(mapChip));
		}
	}
	//ファイルは開いたら、ちゃんと閉じる。。
	fclose(fp);
}
void Level::Draw()
{
	for (auto& mapChip : m_mapChipArray) {
		mapChip->Draw();
	}
}