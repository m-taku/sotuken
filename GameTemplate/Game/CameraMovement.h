#pragma once
class CameraMovement
{
public:
	CameraMovement();
	~CameraMovement();

	void DefaultMove(const CVector3& target, const CVector3& move = CVector3::Zero(), const CVector3& forward = CVector3::Front(), const CVector3& right = CVector3::Right(), const CVector3& up = CVector3::Up());
private:
	CVector3 SpringExecute(const CVector3& toCameraPos, const CVector3& TargetPos);
	CVector3 m_forward = CVector3::Front();
	CVector3 m_right = CVector3::Right();
	CVector3 m_up = CVector3::Up();
	const float MAXDIST = 500.0f;					//ÉJÉÅÉâÇÃç≈ëÂãóó£
	float m_dist = MAXDIST;
};

