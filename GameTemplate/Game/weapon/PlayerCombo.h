#pragma once
#include"graphics/SkinModelDataManager.h"
class Player;
class PlayerCombo : public Noncopyable
{
public:
	PlayerCombo(Player* player)
	{
		m_player = player;
	}
	virtual ~PlayerCombo() {
	}
	virtual void SetAnimation(Player* player)
	{

	}
	virtual void changeY()
	{

	}
	virtual void pullweapon()
	{

	}
	virtual void pushweapon()
	{

	}
	virtual void changeB()
	{

	}
	virtual void Reset()
	{

	}
	const float GetWeight()
	{
		return m_Weight;
	}
	void changeweapon();
	void changeweapon(int No);
protected:
	std::vector<AnimationClip> m_animClip;
	Player* m_player = nullptr;
	int NowWeapon = 0;
	float m_Weight = 1.0f;
	DirectX::Model*		m_modelDxweapon[2];
};

