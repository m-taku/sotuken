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
				if (model->Culling(delayNo, kaku123))
				{
					model->Draw(delayNo);
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


void SkinModelManager::CalculateFrustumPlanes()
{
	auto n = g_camera3D.GetViewMatrix();

	auto CameraAngle = g_camera3D.GetViewAngle();
	n.Inverse(n);
	// 0: Left, 1: Right, 2: Bottm, 3: Top
	//for (int i = 0; i < 4; i++)
	//{
	//	CVector3 m_pos;
	//	int r = i / 2;
	//	if (i % 2 == 0)
	//	{
	//		// •½–Ê‚Ì•û’öŽ®
	//		// ax + by + cz + d = 0
	//		m_pos.x = pmat.m[3][0] - pmat.m[r][0];
	//		m_pos.y = pmat.m[3][1] - pmat.m[r][1];
	//		m_pos.z = pmat.m[3][2] - pmat.m[r][2];
	//		//m_pos.w = pmat.m[3][3] - pmat.m[r][3];
	//	}
	//	else
	//	{
	//		m_pos.x = pmat.m[3][0] + pmat.m[r][0];
	//		m_pos.y = pmat.m[3][1] + pmat.m[r][1];
	//		m_pos.z = pmat.m[3][2] + pmat.m[r][2];
	//		//m_pos.w = pmat.m[3][3] + pmat.m[r][3];
	//	}
	//	CVector3 normal;
	//	//normal.Cross(leftposdown, leftposup);
	//	
	//	m_pos.Normalize();
	//	normal = m_pos * -1.0f;
	//	CVector3 kaku;
	//	kaku.x = n.m[2][0];//‚Ü‚¦
	//	kaku.y = n.m[2][1];
	//	kaku.z = n.m[2][2];
	//	kaku.Normalize();
	//	//kaku.y = 0.0f;
	//	auto ahaha = kaku;
	//	ahaha.y = 0.0f;
	//	CVector3 jiku;
	//	auto hajiki = ahaha.Dot(CVector3::AxisZ());
	//	if (fabsf(hajiki) < 0.99999f) {
	//		jiku.Cross(CVector3::AxisZ(), ahaha);
	//	}
	//	else
	//	{
	//		
	//		//jiku = CVector3::AxisY();
	//	}
	//	CQuaternion na;
	//	//if (hajiki > 0.0f || hajiki < -FLT_MIN)
	//	//{
	//	//	hajiki = CMath::RadToDeg(hajiki);
	//	//	//CVector3 jiku;
	//	//	jiku.Cross(CVector3::AxisZ(), kaku);
	//	//	//if (jiku.y > 0.0f || jiku.y < -FLT_MIN)
	//	//	{
	//	//		jiku.Normalize();
	//	//		na.SetRotationDeg(jiku, hajiki);
	//	//	}
	//	//}
	//	//auto ahaha = kaku;
	//	ahaha.y = 0.0f;
	//	float m = acosf(min(1.0f,max(-1.0f, ahaha.Dot(CVector3::AxisZ()))));
	//	auto naagreagaeg = CMath::RadToDeg(m);
	//	
	//	
	//	jiku.Normalize();
	//	normal.Normalize();

	//	//normal.z -= 1.0f;
	//	CVector3 popopop;
	//	popopop.x = n.m[3][0];
	//	popopop.y = n.m[3][1];
	//	popopop.z = n.m[3][2];
	//	m_kaku[i].devud_Nomal = normal;
	//	na.SetRotationDeg(jiku, naagreagaeg);
	//	na.Multiply(normal);
	//	normal.Normalize();
	//	//normal = kaku + hiki;
	//	m_kaku[i].devud_front = kaku;
	//	m_kaku[i].m_normal = normal;
	//	m_kaku[i].m_popopop = popopop;
	//}
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
	CVector3 nomal[4];
	nomal[0].Cross(leftposup, leftposdown);
	nomal[1].Cross(Riteposdown, riteposup);
	nomal[2].Cross(leftposdown, Riteposdown);
	nomal[3].Cross(riteposup, leftposup);
	for (int i = 0; i < 4; i++) {
		nomal[i].Normalize();
		kaku123[i].m_normal = nomal[i];
		kaku123[i].m_popopop = popopop;
	}

}