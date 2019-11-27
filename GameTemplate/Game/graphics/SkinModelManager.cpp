#include "stdafx.h"
#include "SkinModelManager.h"
#include"SkinModel.h"
#include"VectorDraw.h"
#include <cassert>
#include <thread>
SkinModelManager::SkinModelManager()
{
	for (int i = 0; i < 5; i++)
	{
		m_vector[i] = new VectorDraw(CVector3::Zero());
		m_vector[i]->Update(CVector3::Zero());
	}
}


SkinModelManager::~SkinModelManager()
{
}
void SkinModelManager::NormalCulling()
{
	std::thread Culling([&] {
		auto delayNo = No;
		delayNo++;
		delayNo %= 2;
		m_finisi = false;
		m_Cullingmodels[delayNo].clear();
		for (auto model : m_models[delayNo])
		{
			if (model->Culling(delayNo))
			{
				m_Cullingmodels[delayNo].push_back(model);
			}
		}
		if (m_models[delayNo].size() != 0) {
			if (g_pad[0].IsPress(enButtonLB3)) {
				for (int i = 0; i <4 ; i++)
				{
					auto mm = *(m_models[delayNo].begin());
					auto pos = mm->m_kaku[i].m_popopop;
					auto mmmm = (mm->m_kaku[i].devud_front*100.0f);
					//pos += mmmm;
					m_vector[i]->Update(pos, mm->m_kaku[i].m_normal, 30.0f*i+50.0f);
					//pos += mm->m_kaku[i].devud_front*75.0f;
					//m_vector[i*2+1]->Update(pos, mm->m_kaku[i].devud_fnt, 15.0f + 50.0f);
				}
				auto mm = *(m_models[delayNo].begin());
				auto pos = mm->m_kaku[0].m_popopop;
				auto mmmm = (mm->m_kaku[0].devud_front*500.0f);
				//pos += mmmm;
				//m_vector[4] = new VectorDraw(CVector3::Zero());
				m_vector[4]->Update(pos, mm->m_kaku[0].devud_front, 1000.0f);
				m_vector[4]->Draw();
				//pos.z += len;
				//m_vector[2]->Update(pos, mm->m_kaku[0].devud_Nomal, 20.0f);
				//m_vector[2]->Draw();
			}
			for (int i = 0; i < 5; i++)
			{
				m_vector[i]->Draw();
			}
		}
		for (auto model : m_Cullingmodels[delayNo])
		{
			if (m_Cullingmodels[delayNo].size() == m_models[delayNo].size())
			{
				int i = 0;
			}
			model->Draw(delayNo);
		}
		m_models[delayNo].clear();
		m_finisi = true;
	});
	Culling.detach();
}