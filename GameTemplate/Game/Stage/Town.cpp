#include "stdafx.h"
#include "Town.h"
#include"TEstNPC.h"
#include"../Enemyname/MonsterList.h"
#include"Player.h"
#include"Quest_NPC.h"
#include"../Quest_NPC.h"


Town::Town()
{
}


Town::~Town()
{
	DeleteGO(TestNPC);
}
bool Town::Init()
{
	m_Name = L"Town";
#ifdef DenugWorld
	//地面だけ名前のみ
	wchar_t moveFilePath[256];
	swprintf_s(moveFilePath, L"Assets/modelData/%s.cmo", m_Name);
	/*m_model.Init(L"Assets/modelData/unityChan.cmo");
	m_poa.CreateMeshObject(m_model, {10000.0f,1000000.0f,0.0f}, CQuaternion::Identity());*/
	swprintf_s(moveFilePath, L"Assets/level/%s.tkl", m_Name);
	nra.UEInit(moveFilePath, [&](LevelObjectData& objData) {
		int result = 1;
		result = wcscmp(L"Sphere", objData.name);
		if (result == 0)
		{
			return true;
		}
		result = wcscmp(L"Cube", objData.name);
		if (result == 0)
		{
			auto pos = objData.position;
			auto player = FindGO<Player>("player");
			player->SetPosition(pos);
			return true;
		}
		result = wcscmp(L"unityChan", objData.name);
		if (result == 0)
		{
			TestNPC = NewGO<Quest_NPC>(0, "受付ジョー");
			TestNPC->Setposition(objData.position);
			return true;
		}
		result = wcscmp(L"SM_Grass01", objData.name);
		if (result == 0)
		{
			objData.m_Transflag = true;
			objData.m_Physicsflag = false;
			objData.m_www = true;
			return false;
		}
		result = wcscmp(L"SM_Grass02", objData.name);
		if (result == 0)
		{
			objData.m_Transflag = true;
			objData.m_Physicsflag = false;
			objData.m_www = true;
			return false;
		}
		result = wcscmp(L"SM_Grass03", objData.name);
		if (result == 0)
		{
			objData.m_Transflag = true;
			objData.m_Physicsflag = false;
			objData.m_www = true;
			return false;
		}
		return false;
	});
	swprintf_s(moveFilePath, L"Assets/modelData/%scori1.cmo", m_Name);
	//m_testmodel.Init(moveFilePath);
	/*m_physicsStaticObject.CreateMeshObject(m_testmodel, CVector3::Zero(), CQuaternion::Identity());
	m_physicsStaticObject.GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_Object);*/

	//レベルデザインはここで
#else


#endif 
	return true;
}
void Town::DrawDebug()
{
	//nra.Updata();
	nra.Draw();
}