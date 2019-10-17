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
		*@brief	�������B
		*@param[in]	gameObjectPrioMax	�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l�B(255�܂�)
		*/
		void Init(int gameObjectPrioMax);
		/*!
		*@brief	�Q�[���I�u�W�F�N�g�̖��O�L�[���쐬�B
		*/
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = Util::MakeHash("Undefined");	//���O�L�[�B
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
		*@brief	�C���X�^���X�̎擾�B
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
			//TK_ASSERT(prio <= m_gameObjectPriorityMax, "�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔���傫�����܂��B");
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
						//�������B
						T* p = dynamic_cast<T*>(go);
						return p;
					}
				}
			}
			return nullptr;
		}
		void DeleteExecution();
	private:
		typedef std::list<IGameObject*>	GameObjectList;						//�Q�[���I�u�W�F�N�g�̃��X�g
		std::vector<GameObjectList> m_gameObjectListArray;					//�D��x�t���̃Q�[���I�u�W�F�N�g�̃��X�g
		std::vector<GameObjectList> m_deleteObjectArray[2];					//�f���[�g�\��̃Q�[���I�u�W�F�N�g�̃��X�g�i�f���[�g����Delete���Ȃ��悤��2�p�Ӂj
		std::map<IGameObject*, GameObjectPrio> m_newList;					//new�̍ۂ̕ۑ��ꏊ�inewGO�̏����𖳎����ē����ɂ��邽�߁j
		GameObjectPrio				m_gameObjectPriorityMax;				//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔�B
		int m_currentDeleteObjectBufferNo = 0;								//!<���݂̍폜�I�u�W�F�N�g�̃o�b�t�@�ԍ��B
		static const unsigned char 			GAME_OBJECT_PRIO_MAX = 255;		//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l�B
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

