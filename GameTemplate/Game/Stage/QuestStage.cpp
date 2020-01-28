#include "stdafx.h"
#include "QuestStage.h"
#include"Player.h"
#include"../Enemyname/MonsterList.h"
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
	swprintf_s(moveFilePath, L"Assets/modelData/%s1.cmo", m_Name);
	m_model.Init(moveFilePath);
	m_poa.CreateMeshObject(m_model, CVector3::Zero(), CQuaternion::Identity());
	m_poa.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_Object);
	swprintf_s(moveFilePath, L"Assets/level/%s3.tkl", m_Name);
	int count = 0;
	nra.UEInit(moveFilePath, [&](LevelObjectData& objData) {
		int result = 1;
		result = wcscmp(L"rock_boulder_b1", objData.name);
		if (result == 0)
		{
			return false;
		}
		result = wcscmp(L"rock_boulder_b2", objData.name);
		if (result == 0)
		{
			return false;
		}	
		result = wcscmp(L"SM_Rock05", objData.name);
		if (result == 0)
		{
			return false;
		}
		result = wcscmp(L"Cube", objData.name);
		if (result == 0)
		{
			auto pos = objData.position;
			pos.y += 100.0f;
			FindGO<Player>("player")->SetPosition(pos);
			pos.y += -150.0f;
			new dorakomesu(pos);
			//FindGO<Enemy>("dorakomesu")->SetPosition(pos);
			return true;
		}
		result = wcscmp(L"unityChan", objData.name);
		if (result == 0)
		{
			//NewGO<Enemy>(0, "enemy")->SetPosition(objData.position);
			return true;
		}	
		result = wcscmp(L"SM_Tree01", objData.name);
		if (result == 0)
		{
			objData.m_furag = true;
			return false;
		}
		result = wcscmp(L"SM_Tree02", objData.name);
		if (result == 0)
		{
			objData.m_furag = true;
			return false;
		}
		result = wcscmp(L"SM_Tree03", objData.name);
		if (result == 0)
		{
			objData.m_furag = true;
			return false;
		}
		result = wcscmp(L"SM_Tree04", objData.name);
		if (result == 0)
		{
			objData.m_furag = true;
			return false;
		}
		return true;
	});
	swprintf_s(moveFilePath, L"Assets/level/www.tkl", m_Name);
	nra1.Init(moveFilePath, [&](LevelObjectData& objData) {

		objData.m_furag = true;
		int result = 1;
		result = wcscmp(L"SM_Bush02", objData.name);
		if (result == 0)
		{
			//NewGO<Enemy>(0, "enemy")->SetPosition(objData.position);
			return false;
		}
		return false;
	});
	//swprintf_s(moveFilePath, L"Assets/modelData/%scori.cmo", m_Name);
	//m_testmodel.Init(moveFilePath);
	//m_physicsStaticObject.CreateMeshObject(m_testmodel, CVector3::Zero(), CQuaternion::Identity());
	m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	//m_physicsStaticObject.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_Object);
#else

#endif // DEBUG
	return true;
}
void QuestStage::DrawDebug()
{
	nra.Draw();
	nra1.Draw();
	m_model.Draw(enNormal,g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
}