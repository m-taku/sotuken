#pragma once
class SkinModel;
class VectorDraw;

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
	private:

		VectorDraw* m_vector[5];
		std::list<SkinModel*> m_models[2];
		std::list<SkinModel*> m_Cullingmodels[2];
		int No = 0;
		bool m_finisi = false;
		float len = 10.0f;
	};
	static SkinModelManager& GetSkinModelManager()
	{
		return SkinModelManager::Instance();
	}
}
