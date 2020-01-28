#pragma once

#include "physics/PhysicsStaticObject.h"

class Level;
struct LevelObjectData;

/*!
* @brief	�}�b�v�`�b�v�B
*/
class MapChip {
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*@param[in] objData			�I�u�W�F�N�g���B
	*/
	MapChip(const LevelObjectData& objData);
	~MapChip();
	void Init(const LevelObjectData& objData);
	void State();
	/*!
	* @brief	�`��B
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
	SkinModel m_model;		//!<���f���B
	wchar_t filePath[256];
	bool m_Shadow = true;
	bool m_www = false;
	std::vector<LevelObjectData> m_LevelData;
	std::vector<PhysicsStaticObject*> m_physicsStaticObject;	//!<�ÓI�����I�u�W�F�N�g�B
	
};