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
		/*m_directioinLight.SetColor(CVector4::White());
		m_directioinLight.SetDirection(CVector3::Down());*/
		m_pointLight.SetColor(CVector4::White());
		m_pointLight.SetPosition(CVector3::Zero() + CVector3::Up()*150.0f);
		m_pointLight.SetAttn({ 300.0f,1.0f,0.0f,0.0f });
		//smLightManager().AddLight(&m_directioinLight);
		smLightManager().AddLight(&m_pointLight);
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
	PointLight m_pointLight;
};

