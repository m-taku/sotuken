#pragma once
#include"physics/PhysicsStaticObject.h"
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
		m_model.Init(m_Name);
		m_poa.CreateMeshObject(m_model, CVector3::Zero(), CQuaternion::Identity());
		m_directioinLight.SetColor(CVector4::White());
		m_directioinLight.SetDirection(CVector3::Down());
		smLightManager().AddLight(&m_directioinLight);
		return true;
	}
	void Update()
	{

		m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());
	}
	void Draw()
	{


		m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	}
protected:
	SkinModel m_model;
	PhysicsStaticObject m_poa;
	wchar_t* m_Name;
	DirectionLight m_directioinLight;
};

