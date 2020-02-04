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
	virtual void DrawDebug()
	{
	}

	bool Start()
	{

#ifdef DenugWorld


		Init();

#else
	
#endif
		return true;
	}
	void Update()
	{
#ifdef DenugWorld
		nra.Updata();
		nra1.Updata();
#else
		m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One()); #endif
#endif
	}
	void Draw()
	{
#ifdef DenugWorld
		DrawDebug();
#else
		nra.Draw();
		m_model.Draw(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
#endif

	}
protected:
	SkinModel m_model;
	SkinModel m_testmodel;
	PhysicsStaticObject m_poa;
	//PhysicsStaticObject m_physicsStaticObject;	//!<静的物理オブジェクト。
	wchar_t* m_Name;
	Level nra;
	Level nra1;
	////仮の対応絶対直せ
	//DirectionLight* m_directioinLight;
	//PointLight* m_pointLight; 
};

