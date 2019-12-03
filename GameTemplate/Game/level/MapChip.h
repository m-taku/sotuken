#pragma once

//#include "physics/PhysicsStaticObject.h"

class Level;
struct LevelObjectData;

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
	wchar_t filePath[256];
	std::vector<LevelObjectData> m_LevelData;
//	PhysicsStaticObject m_physicsStaticObject;	//!<静的物理オブジェクト。
	
};