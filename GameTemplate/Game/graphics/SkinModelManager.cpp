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
	//}
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
				if (model->Culling(enNormal, delayNo, kaku123))
				{
					if (m_Cullingmodels[delayNo].size() > 0) {
						auto num = m_Cullingmodels[delayNo].size() / 2;
						auto hogenow = m_Cullingmodels[delayNo].begin();
						auto hogeMin = m_Cullingmodels[delayNo].begin();
						while (num != 0) {
							for (int i = 0; i < num; i++) {
								hogenow++;
								if (hogenow == m_Cullingmodels[delayNo].end())
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
								if (hogenow == m_Cullingmodels[delayNo].end())
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
						m_Cullingmodels[delayNo].insert(hogenow, model);
					}
					else
					{
						m_Cullingmodels[delayNo].push_back(model);
					}
				}
			}
		}
		for (auto model : m_Cullingmodels[delayNo])
		{
			model->Draw(delayNo);
		}
		m_models[delayNo].clear();
		m_Cullingmodels[delayNo].clear();
		m_finisi = true;
	});
	Culling.detach();
}
const sikaku* SkinModelManager::SCalculateFrustumPlanes(CMatrix mView,float height,float width,float farZ)
{
	auto n = mView;
	n.Inverse(n);
	CVector3 kaku, rite;
	kaku.x = n.m[2][0];//‚Ü‚¦
	kaku.y = n.m[2][1];
	kaku.z = n.m[2][2];
	kaku.Normalize();
	CVector3 popopop;
	popopop.x = n.m[3][0];
	popopop.y = n.m[3][1];
	popopop.z = n.m[3][2];
	rite.x = n.m[0][0];//‰E
	rite.y = n.m[0][1];
	rite.z = n.m[0][2];
	rite.Normalize();
	CVector3 GameCamUp;
	GameCamUp.Cross(kaku, rite);
	GameCamUp.Normalize();


	/*auto lenUp = kaku.Length()* tanf(CameraAngle / 2.0f);
	auto lenrite = lenUp * aspect;

	auto XMAX = (rite * lenrite);
	auto YMAX = (GameCamUp * lenUp);
	auto ZMAX = kaku * 0.5f;

	auto leftposup = YMAX - XMAX + ZMAX;
	auto leftposdown = leftposup - (YMAX*2.0f);
	auto riteposup = leftposup + (XMAX *2.0f);
	auto Riteposdown = leftposdown + (XMAX * 2.0f);
	leftposup.Normalize();
	leftposdown.Normalize();
	riteposup.Normalize();
	Riteposdown.Normalize();
	*/
	kaku123[0].m_normal = rite;
	kaku123[1].m_normal = rite * -1.0f;
	kaku123[2].m_normal = GameCamUp;
	kaku123[3].m_normal = GameCamUp * -1.0f;
	kaku123[4].m_normal = kaku;
	kaku123[5].m_normal = kaku * -1.0f;
	kaku123[0].m_popopop = popopop + (rite * -1.0f)*(width / 2.0f);
	kaku123[1].m_popopop = popopop + rite *(width / 2.0f);
	kaku123[2].m_popopop = popopop + (GameCamUp *-1.0f) * (height / 2.0f);
	kaku123[3].m_popopop = popopop + (GameCamUp) * (height / 2.0f);
	kaku123[4].m_popopop = popopop + kaku * (farZ / 100.0f);
	kaku123[5].m_popopop = popopop + kaku * farZ;
	return kaku123;
}
void SkinModelManager::CalculateFrustumPlanes()
{
	auto n = g_camera3D.GetViewMatrix();

	auto CameraAngle = g_camera3D.GetViewAngle();
	n.Inverse(n);
	CVector3 kaku, rite;
	kaku.x = n.m[2][0];//‚Ü‚¦
	kaku.y = n.m[2][1];
	kaku.z = n.m[2][2];
	kaku.Normalize();
	CVector3 popopop;
	popopop.x = n.m[3][0];
	popopop.y = n.m[3][1];
	popopop.z = n.m[3][2];
	rite.x = n.m[0][0];//‰E
	rite.y = n.m[0][1];
	rite.z = n.m[0][2];
	rite.Normalize();
	float aspect = FRAME_BUFFER_W / FRAME_BUFFER_H;
	CVector3 GameCamUp;
	GameCamUp.Cross(kaku, rite);
	GameCamUp.Normalize();
	auto lenUp = kaku.Length()* tanf(CameraAngle / 2.0f);
	auto lenrite = lenUp * aspect;

	auto XMAX = (rite * lenrite);
	auto YMAX = (GameCamUp * lenUp);
	auto ZMAX = kaku * 0.5f;

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
	nomal[4] = kaku;
	nomal[5] = kaku * -1.0f;
	for (int i = 0; i < 6; i++) {
		nomal[i].Normalize();
		kaku123[i].m_normal = nomal[i];
		kaku123[i].m_popopop = popopop;
		if (i == 4)
		{
			kaku123[i].m_popopop += kaku * g_camera3D.GatNear();
		}
		if (i == 5)
		{
			kaku123[i].m_popopop += kaku * g_camera3D.GetFar();
		}
	}

}