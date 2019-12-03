#pragma once

//#include "physics/PhysicsStaticObject.h"

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
private:
	SkinModel m_model;		//!<���f���B
	wchar_t filePath[256];
	std::vector<LevelObjectData> m_LevelData;
//	PhysicsStaticObject m_physicsStaticObject;	//!<�ÓI�����I�u�W�F�N�g�B
	
};