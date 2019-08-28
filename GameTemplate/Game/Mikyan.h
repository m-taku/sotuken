#pragma once
class Mikyan
{
public:
	Mikyan( const wchar_t* modelName, CVector3 pos, CQuaternion rot );
	~Mikyan();
	void Update();
	void Draw();
private:
	SkinModel m_skinModel;
	CVector3 m_pos;
	CQuaternion m_rot;
	float angle = 0;
};

