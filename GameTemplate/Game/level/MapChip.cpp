#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"
#include "../graphics/SkinModelDataManager.h"

MapChip::MapChip(const LevelObjectData& objData)
{
	swprintf_s(filePath, objData.name);
	m_LevelData.push_back(objData);

	//m_model.Init(filePath);
	//m_model.UpdateWorldMatrix(objData.position, objData.rotation, objData.scale);
	//m_model.EnableShadowCaster(true);
	//静的物理オブジェクトをメッシュコライダーから作成する。
}
MapChip::~MapChip()
{
	for (auto atari : m_physicsStaticObject)
	{
		delete atari;
	}
}
void MapChip::Init(const LevelObjectData& objData)
{
	m_LevelData.push_back(objData);
}
void MapChip::State()
{	
	wchar_t file[256];
	swprintf_s(file, L"Assets/modelData/%s.cmo", filePath);
	m_model.Init(file, m_LevelData.size());
	SkinModel k;
	swprintf_s(file, L"Assets/modelData/%skni.cmo", filePath);
	SkinModel* ka;
	{
		if (k.init(file)) {
			ka = &k;
		}
		else
		{
			ka = &m_model;
		}
	}
	if (m_LevelData.size() <= 1) {
		if (m_LevelData[0].m_Physicsflag) {
			m_physicsStaticObject.push_back(new PhysicsStaticObject);
			m_physicsStaticObject[0]->CreateMeshObject(*ka, m_LevelData[0].position, m_LevelData[0].rotation, m_LevelData[0].scale);
			m_physicsStaticObject[0]->GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_Object);
		}
		m_model.UpdateWorldMatrix(m_LevelData[0].position, m_LevelData[0].rotation, m_LevelData[0].scale);
	}
	else
	{
		for (int i = 0; i < m_LevelData.size();i++) {
			if (m_LevelData[0].m_Physicsflag) {
				m_physicsStaticObject.push_back(new PhysicsStaticObject);
				m_physicsStaticObject[i]->CreateMeshObject(*ka, m_LevelData[i].position, m_LevelData[i].rotation, m_LevelData[i].scale);
				m_physicsStaticObject[i]->GetRigidBody()->GetBody()->setUserIndex(enCollisionAttr_Object);
			}
			m_model.UpdateInstancingData(m_LevelData[i].position, m_LevelData[i].rotation, m_LevelData[i].scale);
		}
	}
	m_model.EnableShadowCaster(m_LevelData[0].m_Shadowflag);
}
void MapChip::Update()
{
	if (m_LevelData.size() <= 1) {
		m_model.UpdateWorldMatrix(m_LevelData[0].position, m_LevelData[0].rotation, m_LevelData[0].scale);
	}
	else {
		m_model.BeginUpdateInstancingData();
		for (auto obj : m_LevelData) {
			if (m_LevelData[0].m_www)
			{
				auto len = obj.position - g_camera3D.GetPosition();
				if (len.Length() >= 5000.0f)
				{
					continue;
				}
			}
			/*if (m_LevelData[0].m_Tree)
			{
				auto len = obj.position - g_camera3D.GetPosition();
				if (len.Length() >= 8000.0f)
				{
					continue;
				}
			}*/
			m_model.UpdateInstancingData(obj.position, obj.rotation, obj.scale);
		}
	}
	
}
void MapChip::Draw()
{
	if (m_LevelData[0].m_Transflag) {
		m_model.Draw(enTree, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}
	else {
		if (m_LevelData.size() <= 1) {
			m_model.Draw(enNormal, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
		}
		else
		{
			m_model.Draw(enInstancing, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
		}
	}
}