#pragma once
#include "HitObject.h"
/// <summary>
/// 簡易な当たり判定
/// </summary>
namespace smEngine {
	class HitManager
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		HitManager()
		{

		}
		/// <summary>
		/// デストラクタ
		/// </summary>
		~HitManager()
		{
			Release();
		}
		/// <summary>
		/// Hitオブジェクトのインスタンスのゲット
		/// </summary>
		/// <returns>
		/// Hitオブジェクトのインスタンスの
		/// </returns>
		static inline HitManager& GetHitObjict()
		{
			static HitManager t;
			return t;
		}
		const HitObject* Create(const CVector3* pos, float radius, std::function<void(float damage, CVector3 ObjDate)> objict, HitObject::objict_Name name)
		{
			auto Object = new HitObject;
			Object->Create(pos, radius, objict, name);
			m_objict.push_back(Object);
			return Object;
		}
		/// <summary>
		/// 当たり判定の発生（球）
		/// </summary>
		/// <param name="pos">
		/// 当たり判定の中心座標
		/// </param>
		/// <param name="Circle">
		/// 当たり判定の球の大きさ
		/// </param>
		/// <param name="damage">
		/// 与えたいダメージの量
		/// </param>
		/// <param name="name">
		/// 当てたい対象
		/// </param>
		/// <returns>
		/// 当たったらtrue
		/// </returns>
		bool HitTest(CVector3 originpos, CVector3 pos, float Circle, float damage, HitObject::objict_Name name)
		{
			bool furag = false;
			for (auto hit : m_objict)
			{
				//当てたい物体を検索する
				if (hit->Getobjict_Name() == name) {
					//当たり判定を行う。
					if (hit->HitTest(originpos,pos, Circle, damage)) {
						furag = true;
					}
				}
			}
			return furag;
		}
		void Deleteobjict(const HitObject* objict)
		{
			m_objict.erase(std::find(m_objict.begin(), m_objict.end(), objict));
			if (objict != nullptr) {
				delete objict;
			}
		}
		void Release()
		{
			for (auto k : m_objict)
			{
				delete k;
			}
			m_objict.clear();
		}
	private:
		std::list<HitObject*> m_objict;
	};
	static HitManager& GetHitObjict()
	{
		return  HitManager::GetHitObjict();
	}
}