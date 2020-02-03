#pragma once
class Rig : public Noncopyable
{
public:
	Rig();
	~Rig();
	void SetBoon(SkinModel& skinmode, const wchar_t* name)
	{
		m_Bone = skinmode.FindBone(name);
		m_pos = m_Bone->Getpos();
	}
	const CVector3 Updete()
	{
		CVector3 pos;
		pos.Set(m_Bone->GetWorldMatrix().v[3]);
		auto len = pos - m_pos;
		m_pos = pos;
		return len;
	}
	void StateRig() {
		m_pos.Set(m_Bone->GetWorldMatrix().v[3]);
	}
	Bone* m_Bone;
	CVector3 m_pos;
};

