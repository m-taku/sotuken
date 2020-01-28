#pragma once

#include "physics/PhysicsStaticObject.h"

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
	void SetShadow(bool furg)
	{
		m_Shadow = furg;
	}
	void Setwww()
	{
		m_www = true;
	}
private:
	SkinModel m_model;		//!<モデル。
	wchar_t filePath[256];
	bool m_Shadow = true;
	bool m_www = false;
	std::vector<LevelObjectData> m_LevelData;
	std::vector<PhysicsStaticObject*> m_physicsStaticObject;	//!<静的物理オブジェクト。
	
};