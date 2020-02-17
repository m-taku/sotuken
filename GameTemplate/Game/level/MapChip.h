#pragma once

#include "physics/PhysicsStaticObject.h"

class Level;
struct LevelObjectData;
class Player;
/*!
* @brief	マップチップ。
*/
class MapChip {
public:
	/*!
	* @brief	コンストラクタ。
	*@param[in] objData			オブジェクト情報。
	*/
	MapChip(const LevelObjectData& objData);
	~MapChip();
	void Init(const LevelObjectData& objData);
	void State();
	/*!
	* @brief	描画。
	*/
	const wchar_t* getName()
	{
		return filePath;
	}
	void Update();
	void Draw();
private:
	SkinModel m_model;		//!<モデル。
	int count = 0;
	wchar_t filePath[256];
	std::vector<LevelObjectData> m_LevelData;
	std::vector<PhysicsStaticObject*> m_physicsStaticObject;	//!<静的物理オブジェクト。
	Player* m_player = nullptr;

};