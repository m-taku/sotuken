#pragma once
#include"physics/PhysicsStaticObject.h"
#include"level/Level.h"
#include "physics/PhysicsStaticObject.h"
class Stage_Base : public IGameObject
{
public:
	Stage_Base();
	~Stage_Base();
	virtual bool Init()
	{
		return true;
	}
	bool Start()
	{
		Init();
		wchar_t moveFilePath[256];
		swprintf_s(moveFilePath, L"Assets/modelData/%s.cmo", m_Name);
		m_model.Init(moveFilePath);
		m_poa.CreateMeshObject(m_model, CVector3::Zero(), CQuaternion::Identity());
		m_directioinLight.SetColor(CVector4::White());
		m_directioinLight.SetDirection(CVector3::Down());
		m_pointLight.SetColor(CVector4::White());
		m_pointLight.SetPosition(CVector3::Zero() + CVector3::Up()*150.0f);
		m_pointLight.SetAttn({ 300.0f,1.0f,0.0f,0.0f });
		smLightManager().AddLight(&m_directioinLight);
		smLightManager().AddLight(&m_pointLight);	
		swprintf_s(moveFilePath, L"Assets/level/%s.tkl", m_Name);
		nra.UEInit(moveFilePath, [&](LevelObjectData objData) {
			int result = wcscmp(L"rock_boulder_b1", objData.name);
			if (result == 0)
			{
				return false;
			}
			int result1 = wcscmp(L"unityChan", objData.name);
			if (result1 == 0)
			{
				return false;
			}
			return true;
		});
		swprintf_s(moveFilePath, L"Assets/modelData/%scori.cmo", m_Name);
		m_testmodel.Init(moveFilePath);
		m_physicsStaticObject.CreateMeshObject(m_testmodel, CVector3::Zero(),CQuaternion::Identity());
		return true;
	}
	void Update()
	{

		m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	}
	void Draw()
	{

		nra.Draw();
		m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}
protected:
	SkinModel m_model;
	SkinModel m_testmodel;
	PhysicsStaticObject m_poa;
	PhysicsStaticObject m_physicsStaticObject;	//!<静的物理オブジェクト。
	wchar_t* m_Name;
	Level nra;
	DirectionLight m_directioinLight;
	PointLight m_pointLight; 
};

