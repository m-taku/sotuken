#include "stdafx.h"
#include "SkinModelManager.h"
#include"SkinModel.h"
#include <cassert>
#include <thread>
SkinModelManager::SkinModelManager()
{
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
		for (auto model : m_Cullingmodels[delayNo])
		{
			model->Draw(delayNo);
		}
		m_models[delayNo].clear();
		m_finisi = true;
	});
	Culling.detach();
}