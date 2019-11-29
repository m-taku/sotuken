#include "stdafx.h"
#include "VectorDraw.h"

#ifdef _DEBUG
VectorDraw::~VectorDraw()
{
}
void VectorDraw::Update(CVector3 posotion, CVector3 Vector, float power)
{
	vector = Vector;
	Power = power;
	//vector.y = 0.0f;
	CQuaternion Rot = CQuaternion::Identity();
	vector.Normalize();
	float kakuo = acos(vector.Dot(CVector3::AxisZ()));
	if (kakuo > 0.0f || kakuo < -FLT_MIN)
	{
		kakuo = CMath::RadToDeg(kakuo);
		CVector3 jiku;
		jiku.Cross(CVector3::AxisZ(), vector);
		//if (jiku.y > 0.0f || jiku.y < -FLT_MIN)
		{
			jiku.Normalize();
			Rot.SetRotationDeg(jiku, kakuo);
		}
	}
	//if (jiku1.x > 0.0f)
	//{
	//	CQuaternion hoge;
	//	hoge.SetRotationDeg(CVector3::AxisX(), kakuo2);
	//	Rot.Multiply(hoge);
	//}
	//else
	//{
	//}
	//	CQuaternion hoge;
	//	hoge.SetRotationDeg(CVector3::AxisX()*-1, kakuo2);
	//	Rot.Multiply(hoge);
	m_position = posotion;
	m_vector.UpdateWorldMatrix(m_position, Rot, { 1.0f,Power*1.0f,1.0f });
}
void VectorDraw::Update(std::vector<CVector3>::iterator posotion, std::vector<CVector3>::iterator& Vector, std::vector<float>::iterator power)
{
	//int count = 0;
	//do {
	//	vector = *Vector;
	//	Power = *power;
	//	vector.y = 0.0f;
	//	CQuaternion Rot;
	//	vector.Normalize();
	//	float kakuo = acos(vector.Dot(CVector3::AxisZ()));
	//	kakuo = CMath::RadToDeg(kakuo);
	//	CVector3 jiku;
	//	jiku.Cross(CVector3::AxisZ(), vector);
	//	if (jiku.y > 0.0f)
	//	{
	//		Rot.SetRotationDeg(CVector3::AxisY(), kakuo);
	//	}
	//	else
	//	{
	//		Rot.SetRotationDeg(CVector3::AxisY()*-1, kakuo);
	//	}
	//	m_position = *posotion;
	//	m_vector.UpdateInstancingData(m_position, Rot, { 5.0f,Power*1.0f,1.0f });
	//	Vector++;
	//	posotion++;
	//	power++;
	//} while (++count<m_count);
}
void VectorDraw::Update(CVector3 posotion)
{
	CQuaternion Rot;
	m_position = posotion;
	Rot.SetRotationDeg(CVector3::AxisX(), -90.0f);
	m_vector.UpdateWorldMatrix(m_position, Rot, {1.0f,20.0f,1.0f});
}
void VectorDraw::Update(const std::vector<CVector3>& posotion)
{
	//CQuaternion Rot;
	//Rot.SetRotationDeg(CVector3::AxisX(), -90.0f);
	//m_vector.BeginUpdateInstancingData();
	//for (auto pos = posotion.begin(); pos < posotion.end(); pos++)
	//{
	//	m_vector.UpdateInstancingData(*pos, Rot, { 1.0f,20.0f,1.0f });
	//}
}
void VectorDraw::Draw()
{
	m_vector.Draw(
		enShadow,
		smGameCamera().GetCameraViewMatrix(),
		smGameCamera().GetCameraProjectionMatrix()
	);
}

#endif