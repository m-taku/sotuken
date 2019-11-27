#include "stdafx.h"
#include "QuestStage.h"
#include"Player.h"
#include"Enemy/Enemy.h"

QuestStage::QuestStage()
{
}


QuestStage::~QuestStage()
{
}
bool QuestStage::Init() {

	m_Name = L"Ground";
#ifdef DenugWorld
	//地面だけ名前のみ
	wchar_t moveFilePath[256];
	swprintf_s(moveFilePath, L"Assets/modelData/%s.cmo", m_Name);
	m_model.Init(moveFilePath);
	m_poa.CreateMeshObject(m_model, CVector3::Zero(), CQuaternion::Identity());
	swprintf_s(moveFilePath, L"Assets/level/%s1.tkl", m_Name);
	int count = 0;
	nra.UEInit(moveFilePath, [&](LevelObjectData objData) {
		int result = 1;
		result = wcscmp(L"rock_boulder_b1", objData.name);
		if (result == 0)
		{
			count++;
			if (count <= 2) {
				return false;
			}
			else {
				return true;
			}
		}
		result = wcscmp(L"Cube", objData.name);
		if (result == 0)
		{
			FindGO<Player>("player")->SetPosition({0.0f,0.0f,0.0f});
			return true;
		}
		result = wcscmp(L"unityChan", objData.name);
		if (result == 0)
		{
			//NewGO<Enemy>(0, "enemy")->SetPosition(objData.position);
			return true;
		}
		return true;
	});
	swprintf_s(moveFilePath, L"Assets/modelData/%scori.cmo", m_Name);
	m_testmodel.Init(moveFilePath);
	m_physicsStaticObject.CreateMeshObject(m_testmodel, CVector3::Zero(), CQuaternion::Identity());
	m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
#else

#endif // DEBUG
	return true;
}
void QuestStage::DrawDebug()
{
	nra.Draw();
	m_model.Draw(enNormal,g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}