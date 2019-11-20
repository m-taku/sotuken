#pragma once
class SkinModel;
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
		std::list<SkinModel*> m_models[2];
		std::list<SkinModel*> m_Cullingmodels[2];
		int No = 0;
		bool m_finisi = false;
	};
	static SkinModelManager& GetSkinModelManager()
	{
		return SkinModelManager::Instance();
	}
}
