#pragma once
#include"physics/PhysicsStaticObject.h"
#include"level/Level.h"
#include "physics/PhysicsStaticObject.h"
#include"../graphics/SkyCube.h"
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
		m_SkyCube = NewGO<SkyCube>(1, "sky");
		m_SkyCube->Init();
		return true;
	}
	void Update()
	{
		bool f = g_graphicsEngine->GetFade()->IsInFade();
		if (g_pad[0].IsTrigger(enButtonA))
		{
			g_graphicsEngine->GetFade()->FadeInStart();
		}
		if (g_pad[0].IsTrigger(enButtonB))
		{
			g_graphicsEngine->GetFade()->FadeOutStart();
		}
#ifdef DenugWorld
		nra.Updata();
		nra1.Updata();
#else
		m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One()); #endif
#endif
			m_SkyCube->Update(CVector3::Zero(), CVector3::One()*30000.0f);
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
	void PostDraw()
	{
	}
protected:
	SkinModel m_model;
	SkinModel m_testmodel;
	PhysicsStaticObject m_poa;
	SkyCube* m_SkyCube = nullptr;
	//PhysicsStaticObject m_physicsStaticObject;	//!<静的物理オブジェクト。
	wchar_t* m_Name;
	Level nra;
	Level nra1;
};

