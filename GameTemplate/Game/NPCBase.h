#pragma once
class Player;
class NPCBase : public IGameObject
{
public:
	NPCBase();
	~NPCBase();
	bool Start();
	virtual bool Init() {
		return true;
	}
	void Update();
	void Setposition(CVector3 pos)
	{
		m_position = pos;
	}
	void Draw()
	{

	}
	virtual void Talk() {};
protected:
	Player* m_player = nullptr;
	CVector3 m_position;
};

