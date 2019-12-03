#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"

MapChip::MapChip(const LevelObjectData& objData)
{
	swprintf_s(filePath, objData.name);
	m_LevelData.push_back(objData);
	//m_model.Init(filePath);
	//m_model.UpdateWorldMatrix(objData.position, objData.rotation, objData.scale);
	//m_model.EnableShadowCaster(true);
	//静的物理オブジェクトをメッシュコライダーから作成する。
	//m_physicsStaticObject.CreateMeshObject(m_model, objData.position, objData.rotation);
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
	if (m_LevelData.size() <= 1) {
		m_model.UpdateWorldMatrix(m_LevelData[0].position, m_LevelData[0].rotation, m_LevelData[0].scale);
	}
	else
	{
		for (auto obj : m_LevelData) {
			m_model.UpdateInstancingData(obj.position, obj.rotation, obj.scale);
		}
	}
	m_model.EnableShadowCaster(true);
}
void MapChip::Update()
{
	if (m_LevelData.size() <= 1) {
		m_model.UpdateWorldMatrix(m_LevelData[0].position, m_LevelData[0].rotation, m_LevelData[0].scale);
	}
	else {
		m_model.BeginUpdateInstancingData();
		for (auto obj : m_LevelData) {
			m_model.UpdateInstancingData(obj.position, obj.rotation, obj.scale);
		}
	}
	
}
void MapChip::Draw()
{
	if (m_LevelData.size() <= 1) {
		m_model.Draw(enNormal, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}
	else
	{
		m_model.Draw(enInstancing, g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}
}