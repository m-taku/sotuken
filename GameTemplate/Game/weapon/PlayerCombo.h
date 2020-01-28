#pragma once
class Player;
class PlayerCombo : public Noncopyable
{
public:
	PlayerCombo()
	{

	}
	virtual ~PlayerCombo() {
	}
	virtual void SetAnimation(Player* player)
	{

	}
	virtual void changeY()
	{

	}
	virtual void changeB()
	{

	}
protected:
	std::vector<AnimationClip> m_animClip;
	Player* m_player = nullptr;
};

