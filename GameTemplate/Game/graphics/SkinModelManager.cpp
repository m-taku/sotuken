#include "stdafx.h"
#include "SkinModelManager.h"
#include"SkinModel.h"
#include"VectorDraw.h"
#include <cassert>
#include <thread>
SkinModelManager::SkinModelManager()
{
	//for (int i = 0; i < 5; i++)
	//{
	//	m_vector[i] = new VectorDraw(CVector3::Zero());
	//	m_vector[i]->Update(CVector3::Zero());
	//}m_Plane
}


SkinModelManager::~SkinModelManager()
{

}
void SkinModelManager::NormalCulling()
{
	CalculateFrustumPlanes();
	m_finisi = false;
	std::thread Culling([&] {
		auto delayNo = No;
		delayNo++;
		delayNo %= 2;
		if (m_models[delayNo].size() > 0) {
			for (auto model : m_models[delayNo])
			{
				if (model->Culling(enNormal, delayNo, m_Plane))
				{
					if (m_cullingModels[delayNo].size() > 0) {
						auto num = m_cullingModels[delayNo].size() / 2;
						auto hogenow = m_cullingModels[delayNo].begin();
						auto hogeMin = m_cullingModels[delayNo].begin();
						while (num != 0) {
							for (int i = 0; i < num; i++) {
								hogenow++;
								if (hogenow == m_cullingModels[delayNo].end())
								{
									hogenow--;
								}
							}
							if (model->Getcameralen() < (*hogenow)->Getcameralen()) {
								hogenow = hogeMin;
							}
							else
							{
								hogenow++;
								if (hogenow == m_cullingModels[delayNo].end())
								{
									hogenow--;
								}
								hogeMin = hogenow;
							}
							num /= 2;
						}
						if (model->Getcameralen() > (*hogenow)->Getcameralen()) {
							hogenow++;
						}
						m_cullingModels[delayNo].insert(hogenow, model);
					}
					else
					{
						m_cullingModels[delayNo].push_back(model);
					}
				}
			}
		}
		for (auto model : m_cullingModels[delayNo])
		{
			model->Draw(delayNo);
		}
		m_models[delayNo].clear();
		m_cullingModels[delayNo].clear();
		m_finisi = true;
	});
	Culling.detach();
}
const Plane* SkinModelManager::SCalculateFrustumPlanes(CMatrix mView,float height,float width,float farZ)
{
	auto ViewMatrix = mView;
	ViewMatrix.Inverse(ViewMatrix);
	CVector3 front, rite;
	front.x = ViewMatrix.m[2][0];//‚Ü‚¦
	front.y = ViewMatrix.m[2][1];
	front.z = ViewMatrix.m[2][2];
	front.Normalize();
	CVector3 popopop;
	popopop.x = ViewMatrix.m[3][0];
	popopop.y = ViewMatrix.m[3][1];
	popopop.z = ViewMatrix.m[3][2];
	rite.x = ViewMatrix.m[0][0];//‰E
	rite.y = ViewMatrix.m[0][1];
	rite.z = ViewMatrix.m[0][2];
	rite.Normalize();
	CVector3 GameCamUp;
	GameCamUp.Cross(front, rite);
	GameCamUp.Normalize();
	m_Plane[0].m_normal = rite;
	m_Plane[1].m_normal = rite * -1.0f;
	m_Plane[2].m_normal = GameCamUp;
	m_Plane[3].m_normal = GameCamUp * -1.0f;
	m_Plane[4].m_normal = front;
	m_Plane[5].m_normal = front * -1.0f;
	m_Plane[0].m_centerPos = popopop + (rite * -1.0f)*(width / 2.0f);
	m_Plane[1].m_centerPos = popopop + rite *(width / 2.0f);
	m_Plane[2].m_centerPos = popopop + (GameCamUp *-1.0f) * (height / 2.0f);
	m_Plane[3].m_centerPos = popopop + (GameCamUp) * (height / 2.0f);
	m_Plane[4].m_centerPos = popopop + front * (farZ / 100.0f);
	m_Plane[5].m_centerPos = popopop + front * farZ;
	return m_Plane;
}
void SkinModelManager::CalculateFrustumPlanes()
{
	auto ViewMatrix = g_camera3D.GetViewMatrix();

	auto CameraAngle = g_camera3D.GetViewAngle();
	ViewMatrix.Inverse(ViewMatrix);
	CVector3 front, rite;
	front.x = ViewMatrix.m[2][0];//‚Ü‚¦
	front.y = ViewMatrix.m[2][1];
	front.z = ViewMatrix.m[2][2];
	front.Normalize();
	CVector3 popopop;
	popopop.x = ViewMatrix.m[3][0];
	popopop.y = ViewMatrix.m[3][1];
	popopop.z = ViewMatrix.m[3][2];
	rite.x = ViewMatrix.m[0][0];//‰E
	rite.y = ViewMatrix.m[0][1];
	rite.z = ViewMatrix.m[0][2];
	rite.Normalize();
	float aspect = FRAME_BUFFER_W / FRAME_BUFFER_H;
	CVector3 GameCamUp;
	GameCamUp.Cross(front, rite);
	GameCamUp.Normalize();
	auto lenUp = front.Length()* tanf(CameraAngle);
	auto lenrite = lenUp * aspect;

	auto XMAX = (rite * lenrite);
	auto YMAX = (GameCamUp * lenUp);
	auto ZMAX = front * 1.0f;

	auto leftposup = YMAX - XMAX + ZMAX;
	auto leftposdown = leftposup - (YMAX*2.0f);
	auto riteposup = leftposup + (XMAX *2.0f);
	auto Riteposdown = leftposdown + (XMAX * 2.0f);
	leftposup.Normalize();
	leftposdown.Normalize();
	riteposup.Normalize();
	Riteposdown.Normalize();
	CVector3 nomal[6];
	nomal[0].Cross(leftposup, leftposdown);
	nomal[1].Cross(Riteposdown, riteposup);
	nomal[2].Cross(leftposdown, Riteposdown);
	nomal[3].Cross(riteposup, leftposup);
	nomal[4] = front;
	nomal[5] = front * -1.0f;
	for (int i = 0; i < 6; i++) {
		nomal[i].Normalize();
		m_Plane[i].m_normal = nomal[i];
		m_Plane[i].m_centerPos = popopop;
		if (i == 4)
		{
			m_Plane[i].m_centerPos += front * g_camera3D.GetNear();
		}
		if (i == 5)
		{
			m_Plane[i].m_centerPos += front * g_camera3D.GetFar();
		}
	}

}