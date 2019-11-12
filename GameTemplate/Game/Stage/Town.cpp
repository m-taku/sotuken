#include "stdafx.h"
#include "Town.h"
#include"TEstNPC.h"


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
	m_model.Init(L"Assets/modelData/unityChan.cmo");
	m_poa.CreateMeshObject(m_model, CVector3::Zero(), CQuaternion::Identity());
	swprintf_s(moveFilePath, L"Assets/level/%s.tkl", m_Name);
	nra.UEInit(moveFilePath, [&](LevelObjectData objData) {
		int result = 1;
		result = wcscmp(L"Sphere", objData.name);
		if (result == 0)
		{
			return true;
		}
		//result = wcscmp(L"SM_StoneWallBorder3m01", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_Fabric01", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_StreetLight02", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_House1stFloor03", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_Barrel03", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_Barrel04", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_WoodStairs03", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_WoodChunks01", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_CastleWindow02", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_CastleWindow03", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_Floor01", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		//result = wcscmp(L"SM_Floor02", objData.name);
		//if (result == 0)
		//{
		//	return true;
		//}
		result = wcscmp(L"unityChan", objData.name);
		if (result == 0)
		{
			TestNPC = NewGO<TEstNPC>(0, "dnea");
			TestNPC->SetPos(objData.position);
			return true;
		}
		result = wcscmp(L"SM_Grass01", objData.name);
		if (result == 0)
		{
			return true;
		}
		result = wcscmp(L"SM_Grass02", objData.name);
		if (result == 0)
		{
			return true;
		}
		result = wcscmp(L"SM_Grass03", objData.name);
		if (result == 0)
		{
			return true;
		}
		return false;
	});
	swprintf_s(moveFilePath, L"Assets/modelData/%scori.cmo", m_Name);
	m_testmodel.Init(moveFilePath);
	m_physicsStaticObject.CreateMeshObject(m_testmodel, CVector3::Zero(), CQuaternion::Identity());

	//レベルデザインはここで
#else

#endif // DenugWorld


	return true;
}
void Town::DrawDebug()
{
	nra.Draw();
}