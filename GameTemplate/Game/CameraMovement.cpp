#include "stdafx.h"
#include "CameraMovement.h"

CameraMovement::CameraMovement()
{
}


CameraMovement::~CameraMovement()
{
}

namespace {
	//衝突したときに呼ばれる関数オブジェクト(壁用)
	struct RayResultWall : public btCollisionWorld::ClosestRayResultCallback
	{
		RayResultWall(const btVector3&	rayFromWorld, const btVector3&	rayToWorld) :ClosestRayResultCallback(rayFromWorld, rayToWorld)
		{
			m_rayFromWorld = rayFromWorld;
			m_rayToWorld = rayToWorld;
			startPos.Set(m_rayFromWorld);
		}
		btVector3 m_rayFromWorld;
		btVector3 m_rayToWorld;
		bool isHit = false;						//衝突フラグ。
		CVector3 hitPos = CVector3::Zero();		//衝突点。
		CVector3 startPos = CVector3::Zero();		//レイの始点。
		float dist = FLT_MAX;					//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
		CVector3 hitNormal = CVector3::Zero();	//衝突点の法線。
												//衝突したときに呼ばれるコールバック関数。
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Character) {
				//自分に衝突した。
				return 0.0f;
			}
			//衝突点の法線を引っ張ってくる。
			CVector3 hitNormalTmp;
			hitNormalTmp.Set(rayResult.m_hitNormalLocal);
			if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Ground ||
				rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Object)
			{
				isHit = true;


				btVector3	hitPointWorld;
				hitPointWorld.setInterpolate3(m_rayFromWorld, m_rayToWorld, rayResult.m_hitFraction);
				CVector3 buckUpPos = hitPos;
				hitPos.Set(hitPointWorld);
				//交点との距離を調べる。
				CVector3 vDist;
				vDist.Subtract(hitPos, startPos);
				float distTmp = vDist.Length();
				if (distTmp < dist) {
					//この衝突点の方が近いので、最近傍の衝突点を更新する。

					dist = distTmp;
					hitNormal = hitNormalTmp;
					hitNormal.Normalize();
				}
				else {
					hitPos = buckUpPos;
				}
			}
			return 0.0f;
		}
	};
}

CVector3 CameraMovement::SpringExecute(const CVector3& toCameraPos, const CVector3& TargetPos)
{
	CVector3 toCameraVec = toCameraPos;
	CVector3 cameraTarget = TargetPos;
	toCameraVec.Normalize();
	toCameraVec *= MAXDIST;
	CVector3 cameraPosition = cameraTarget + toCameraVec;
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin(btVector3(cameraTarget.x, cameraTarget.y, cameraTarget.z));
	end.setOrigin(btVector3(cameraPosition.x, cameraPosition.y, cameraPosition.z));


	RayResultWall callback(start.getOrigin(), end.getOrigin());
	g_physics.GetDynamicWorld()->rayTest(start.getOrigin(), end.getOrigin(), callback);
	if (callback.isHit)
	{
		CVector3 vec = toCameraVec;
		vec.Normalize();
		CVector3 v = toCameraVec - (vec * callback.dist);
		CVector3 aX = ((callback.hitNormal*-1.0f)*(callback.hitNormal*-1.0f).Dot(v)) - v;
		aX.Normalize();
		CVector3 aY = callback.hitNormal;
		v *= -1.0f;
		v.Normalize();
		float a = aY.Dot(v)*(1.0f / aX.Dot(v));
		float x = (1.0f - 0.0f) / (a - 0.0f);
		float y = ((a*1.0f) - (0.0f*0.0f)) / (a - 0.0f);
		float len = CVector3{ x,y,0.0f }.Length();
		CVector3 toHitPos = callback.hitPos- cameraTarget;
		toCameraVec = toHitPos + v * (len*1.0f);
		cameraPosition = cameraTarget + toCameraVec;
	}
	return cameraPosition;
}

void CameraMovement::DefaultMove(const CVector3 & target, const CVector3& move, const CVector3 & forward, const CVector3 & right, const CVector3 & up)
{
	float speed = 200.0f;
	smGameCamera().SetTarget((CVector3)target);
	float Input_RX = g_pad[0].GetRStickXF()*speed*GetGameTime().GetFrameTime();
	float Input_RY = -g_pad[0].GetRStickYF()*speed*GetGameTime().GetFrameTime();
	CVector3 cameraPos = smGameCamera().GetCameraPosition() + move;
	CVector3 cameraTarget = smGameCamera().GetCameraTarget();
	CVector3 toCameraPos = cameraPos - cameraTarget;
	toCameraPos.Normalize();

	CQuaternion qRot = CQuaternion::Identity();
	qRot.SetRotationDeg(smGameCamera().GetCameraUp(), Input_RX);
	qRot.Multiply(toCameraPos);
	CVector3 buckupVec = toCameraPos;
	qRot.SetRotationDeg(smGameCamera().GetCameraRight(), Input_RY);
	qRot.Multiply(toCameraPos);
	float angle = smGameCamera().GetCameraUp().Dot(toCameraPos);
	float absAngle = fabsf(angle);
	float maxA = 0.8f;
	float mini = 0.0002f;
	float n = (angle + mini) / (absAngle + mini);
	float sub = (max(absAngle, maxA) - maxA) * -n;
	qRot.SetRotation(smGameCamera().GetCameraRight(), sub);
	qRot.Multiply(toCameraPos);
	//ifなんか使わないぜ！！！！！！！！！！！！！！
	smGameCamera().SetPosition(SpringExecute(toCameraPos, cameraTarget));

}
