#pragma once

#include"PhysicsDebugDraw.h"

class RigidBody;
class CharacterController;
class PhysicsStaticObject;

class PhysicsWorld
{
	btDefaultCollisionConfiguration*		collisionConfig = nullptr;
	btCollisionDispatcher*					collisionDispatcher = nullptr;	//!<衝突解決処理。
	btBroadphaseInterface*					overlappingPairCache = nullptr;	//!<ブロードフェーズ。衝突判定の枝切り。
	btSequentialImpulseConstraintSolver*	constraintSolver = nullptr;		//!<コンストレイントソルバー。拘束条件の解決処理。
	btDiscreteDynamicsWorld*				dynamicWorld = nullptr;			//!<ワールド。
	PhysicsDebugDraw						m_debugDraw;					//!<デバック用ポリゴン線表示用クラス
public:
	~PhysicsWorld();
	void Init();
	void Update();
	void DebubDrawWorld();
	void Release();
	/*!
	* @brief	ダイナミックワールドを取得。
	*/
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return dynamicWorld;
	}
	/*!
	* @brief	剛体を登録。
	*/
	void SetDebugDrawMode(int debugMode)
	{
		//ワイヤーフレーム描画のみ。
		m_debugDraw.setDebugMode(debugMode);
	}
	void AddRigidBody(RigidBody& rb);
	/*!
	* @brief	剛体を破棄。
	*/
	void RemoveRigidBody(RigidBody& rb);
	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f
	)
	{
		dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}
	//void ContactText(
	//	btCollisionObject* colObj, 
	//	btCollisionWorld::ContactResultCallback& resultCallback
	//)
	//{
	//	dynamicWorld->contactTest(colObj, resultCallback);
	//}

	void ContactTest(
		btCollisionObject* colObj,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
	void ContactTest(
		RigidBody& rb,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
	void ContactTest(
		CharacterController& charaCon,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
	void ContactTest(
		PhysicsStaticObject& physicsstaticobject,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
};

extern PhysicsWorld g_physics;


