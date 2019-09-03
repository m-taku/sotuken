#include "stdafx.h"
#include "IGameObjectManager.h"

namespace GameObject2 {

	IGameObjectManager::IGameObjectManager()
	{
	}


	IGameObjectManager::~IGameObjectManager()
	{
	}
	void IGameObjectManager::Init(int gameObjectPrioMax) {
		if (gameObjectPrioMax > GAME_OBJECT_PRIO_MAX)
		{
			gameObjectPrioMax = GAME_OBJECT_PRIO_MAX;
		}
		m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}
	void IGameObjectManager::Execute()
	{
		for (auto ObjectList:m_gameObjectListArray) {
			for (auto Object : ObjectList)
			{
				Object->StartWrapper();
			}
		}	
		for (auto ObjectList : m_gameObjectListArray) {
			for (auto Object : ObjectList)
			{
				Object->UpdateWrapper();
			}
		}
		for (auto ObjectList : m_gameObjectListArray) {
			for (auto Object : ObjectList)
			{
				Object->DrawWrapper();
			}
		}
		DeleteExecution();
	}
	void IGameObjectManager::DeleteExecution() {
		auto nowNo = m_currentDeleteObjectBufferNo;
		m_currentDeleteObjectBufferNo = ++m_currentDeleteObjectBufferNo % 2;
		for (GameObjectList& DeleteList : m_deleteObjectArray[nowNo]) {
			for (IGameObject* Deleteobject : DeleteList) {
				GameObjectPrio prio = Deleteobject->GetPriority();
				GameObjectList& goExecList = m_gameObjectListArray.at(prio);
				auto it = std::find(goExecList.begin(), goExecList.end(), Deleteobject);
				if (it != goExecList.end()) {
					//削除リストから除外された。
					(*it)->m_isRegistDeadList = false;
					delete (*it);
					
				}
				goExecList.erase(it);
			}
			DeleteList.clear();
		}
		//m_DeleteList[nowNo].clear();
	}
}
