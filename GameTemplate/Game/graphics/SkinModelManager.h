#pragma once
class SkinModel;
//class VectorDraw;
struct Plane
{
	CVector3 m_normal;
	CVector3 m_centerPos;
};
namespace smEngine {

	class SkinModelManager :public Noncopyable
	{
	public:
		SkinModelManager();
		~SkinModelManager();
		static inline SkinModelManager& Instance()
		{
			static SkinModelManager Instance;
			return Instance;
		}
		void NormalCulling();
		void SetModel(SkinModel* model)
		{
			m_models[No].push_back(model);
		}
		bool Getfafa()
		{
			return m_finisi;
		}
		int GetNo()
		{
			return No;
		}
		void SetNo(int num)
		{
			No = num;
		}
		const Plane* SCalculateFrustumPlanes(CMatrix mView, float height, float width, float farZ);
		void CalculateFrustumPlanes();
	private:
/*
		VectorDraw* m_vector[5];*/
		Plane m_Plane[6];
		std::list<SkinModel*> m_models[2];
		std::list<SkinModel*> m_cullingModels[2];
		int No = 0;
		bool m_finisi = false;
		float len = 10.0f;
	};
	static SkinModelManager& GetSkinModelManager()
	{
		return SkinModelManager::Instance();
	}
}
