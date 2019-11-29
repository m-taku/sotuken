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
	//�X�P���g�������[�h����B
	Skeleton skeleton;
	skeleton.Load(levelDataFilePath);
	//�\�z�\�z�B
	//0�Ԗڂ̓��[�g�I�u�W�F�N�g�Ȃ̂Ŕ�΂��B
	for (auto i = 1; i < skeleton.GetNumBones(); i++) {
		//�����擾�B
		auto bone = skeleton.GetBone(i);
		if (bone->GetParentId() == 0) {	//�e�����[�g�̏ꍇ�����}�b�v�`�b�v�𐶐�����B
			LevelObjectData objData;
			CVector3 scale;
			bone->CalcWorldTRS(objData.position, objData.rotation, scale);
			//3dsMax�Ƃ͎����Ⴄ�̂ŁA�␳������B
			auto t = objData.position.y;
			objData.position.y = objData.position.z;
			objData.position.z = -t;

			t = objData.rotation.y;
			objData.rotation.y = objData.rotation.z;
			objData.rotation.z = -t;
			objData.name = bone->GetName();
			bool isHook = false;
			if (hookFunc != nullptr) {
				//hook�֐����w�肳��Ă���̂�hook�֐����Ăяo���B
				isHook = hookFunc(objData);
			}
			if (isHook == false) {
				//�t�b�N����Ȃ������̂ŁA�}�b�v�`�b�v���쐬����B
				auto mapChip = std::make_unique<MapChip>(objData);
				m_mapChipArray.push_back(std::move(mapChip));
			}
		}
	}
}
void Level::UEInit(const wchar_t* levelDataFilePath, Level::HookWhenBuildObjectFunc hookFunc)
{
	//tks�t�@�C�����I�[�v���B
	FILE* fp = _wfopen(levelDataFilePath, L"rb");
	if (fp == nullptr) {
		return;
	}
	//���̐����擾�B
	int numBone = 0;
	//tks�t�@�C���̐擪����A4�o�C�g�ǂݍ��ށB
	//�擪�S�o�C�g�ɍ��̐��̃f�[�^�������Ă���B
	fread(&numBone, sizeof(numBone), 1, fp);
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		LevelObjectData objData;
		int namelen;
		fread(&namelen, sizeof(int), 1, fp);
		//���̖��O��ǂݍ��߂邾���̃��������m�ۂ���B
		char* name = new char[namelen + 1];
		//���̖��O��ǂݍ��݁B+1�͏I�[�������ǂݍ��ނ���
		fread(name, namelen + 1, 1, fp);

		size_t kosuu;
		setlocale(LC_ALL, "japanese");
		wchar_t boneName[256];
		mbstowcs(boneName, name, 256);
		objData.name = boneName;
		//��]����ǂݍ���
		fread(&objData.rotation, sizeof(CQuaternion), 1, fp);
		//objData.rotation = CQuaternion::Identity();
		//�ړ�����ǂݍ���
		CVector4 pos = CVector4::White();
		fread(&pos, sizeof(CVector4), 1, fp);
		objData.position.x = pos.x;
		objData.position.y = pos.y;
		objData.position.z = pos.z;
		//�g�����ǂݍ���
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
			//hook�֐����w�肳��Ă���̂�hook�֐����Ăяo���B
			isHook = hookFunc(objData);
		}
		if (isHook == false) {
			//�t�b�N����Ȃ������̂ŁA�}�b�v�`�b�v���쐬����B
			auto mapChip = std::make_unique<MapChip>(objData);
			m_mapChipArray.push_back(std::move(mapChip));
		}
	}
	//�t�@�C���͊J������A�����ƕ���B�B
	fclose(fp);
}
void Level::Draw()
{
	for (auto& mapChip : m_mapChipArray) {
		mapChip->Draw();
	}
}