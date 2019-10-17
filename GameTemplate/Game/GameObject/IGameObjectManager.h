#pragma once
#include"util/Util.h"
#include"IGameObject.h"
namespace smEngine {
	class IGameObjectManager:Noncopyable
	{
	public:
		IGameObjectManager();
		~IGameObjectManager();
		/*!
		*@brief	初期化。
		*@param[in]	gameObjectPrioMax	ゲームオブジェクトの優先度の最大値。(255まで)
		*/
		void Init(int gameObjectPrioMax);
		/*!
		*@brief	ゲームオブジェクトの名前キーを作成。
		*/
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = Util::MakeHash("Undefined");	//名前キー。
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = Util::MakeHash(objectName);
			}
			return hash;
		}
		void Execute();
		/*!
		*@brief	インスタンスの取得。
		*/
		static IGameObjectManager& Instance()
		{
			static IGameObjectManager instance;
			return instance;
		}
		void DeleteGameObject(IGameObject * gameObject)
		{
			if (gameObject != nullptr
				) {
				//gameObject->SetDeadMark();
				gameObject->OnDestroy();
				gameObject->m_isRegistDeadList = true;
				gameObject->m_isDead = true;
				m_deleteObjectArray[m_currentDeleteObjectBufferNo].at(gameObject->GetPriority()).push_back(gameObject);
				gameObject = nullptr;
			}
		}
		template<class T>
		T* NewGameObject(GameObjectPrio prio, const char* objectName)
		{
			(void*)objectName;
			//TK_ASSERT(prio <= m_gameObjectPriorityMax, "ゲームオブジェクトの優先度の最大数が大きすぎます。");
			T* newObject = new T;
			m_newList.insert({ newObject,prio });
			unsigned int hash = MakeGameObjectNameKey(objectName);
			newObject->m_priority = prio;
			newObject->m_nameKey = hash;
			return newObject;
		}
		void NewExecution()
		{
			for (auto Newobject : m_newList)
			{
				m_gameObjectListArray.at(Newobject.second).push_back(Newobject.first);
			}
			m_newList.clear();
		}
		template<class T>
		T* FindGameObject(const char* objectName)
		{
			unsigned int nameKey = Util::MakeHash(objectName);
			for (auto goList : m_gameObjectListArray) {
				for (auto go : goList) {
					if (go->m_nameKey == nameKey) {
						//見つけた。
						T* p = dynamic_cast<T*>(go);
						return p;
					}
				}
			}
			return nullptr;
		}
		void DeleteExecution();
	private:
		typedef std::list<IGameObject*>	GameObjectList;						//ゲームオブジェクトのリスト
		std::vector<GameObjectList> m_gameObjectListArray;					//優先度付きのゲームオブジェクトのリスト
		std::vector<GameObjectList> m_deleteObjectArray[2];					//デリート予定のゲームオブジェクトのリスト（デリート中にDeleteしないように2つ用意）
		std::map<IGameObject*, GameObjectPrio> m_newList;					//newの際の保存場所（newGOの順序を無視して同時にするため）
		GameObjectPrio				m_gameObjectPriorityMax;				//!<ゲームオブジェクトの優先度の最大数。
		int m_currentDeleteObjectBufferNo = 0;								//!<現在の削除オブジェクトのバッファ番号。
		static const unsigned char 			GAME_OBJECT_PRIO_MAX = 255;		//!<ゲームオブジェクトの優先度の最大値。
	};
	static IGameObjectManager& GameObjectManager()
	{
		return IGameObjectManager::Instance();
	}
	template<class T>
	static inline T* FindGO(const char* objectName)
	{
		return GameObjectManager().FindGameObject<T>(objectName);
	}
	template<class T>
	static inline T* NewGO(int priority, const char* objectName = nullptr)
	{
		return GameObjectManager().NewGameObject<T>((GameObjectPrio)priority, objectName);
	}
	static inline void DeleteGO(IGameObject* object)
	{
		return GameObjectManager().DeleteGameObject(object);
	}
}

