#include "stdafx.h"
#include "CameraMovement.h"

CameraMovement::CameraMovement()
{
}


CameraMovement::~CameraMovement()
{
}

void CameraMovement::DefaultMove(const CVector3 & target, const CVector3& move, const CVector3 & forward, const CVector3 & right, const CVector3 & up)
{
	float speed = 200.0f;
	smGameCamera().SetTarget((CVector3)target);
	float Input_RX = g_pad[0].GetRStickXF()*speed*GetGameTime().GetFrameTime();
	float Input_RY = -g_pad[0].GetRStickYF()*speed*GetGameTime().GetFrameTime();
	CVector3 cameraPos = smGameCamera().GetCameraPosition();
	CVector3 cameraTarget = smGameCamera().GetCameraTarget();
	CVector3 toCameraPos = cameraPos - cameraTarget;
	toCameraPos.Normalize();

	CQuaternion qRot = CQuaternion::Identity();
	qRot.SetRotationDeg(smGameCamera().GetCameraUp(), Input_RX);
	qRot.Multiply(toCameraPos);
	CVector3 buckupVec = toCameraPos;
	qRot.SetRotationDeg(smGameCamera().GetCameraRight(), Input_RY);
	qRot.Multiply(buckupVec);
	float angle = smGameCamera().GetCameraUp().Dot(buckupVec);
	if (angle < 0.999f && angle > -0.999f)
	{
		toCameraPos = buckupVec;
	}
	smGameCamera().SetPosition(cameraTarget + move + toCameraPos * m_dist);

}
