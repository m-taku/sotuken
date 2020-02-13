#include "stdafx.h"
#include "CameraMovement.h"

CameraMovement::CameraMovement()
{
}


CameraMovement::~CameraMovement()
{
}

namespace {
	//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
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
		bool isHit = false;						//�Փ˃t���O�B
		CVector3 hitPos = CVector3::Zero();		//�Փ˓_�B
		CVector3 startPos = CVector3::Zero();		//���C�̎n�_�B
		float dist = FLT_MAX;					//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
		CVector3 hitNormal = CVector3::Zero();	//�Փ˓_�̖@���B
												//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			if (rayResult.m_collisionObject->getUserIndex() == enCollisionAttr_Character) {
				//�����ɏՓ˂����B
				return 0.0f;
			}
			//�Փ˓_�̖@�������������Ă���B
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
				//��_�Ƃ̋����𒲂ׂ�B
				CVector3 vDist;
				vDist.Subtract(hitPos, startPos);
				float distTmp = vDist.Length();
				if (distTmp < dist) {
					//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B

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
	//if�Ȃ񂩎g��Ȃ����I�I�I�I�I�I�I�I�I�I�I�I�I�I
	smGameCamera().SetPosition(SpringExecute(toCameraPos, cameraTarget));

}
