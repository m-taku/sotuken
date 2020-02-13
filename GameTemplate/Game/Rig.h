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
		CMatrix ja;
		ja.Mul(m_Bone->GetLocalMatrix(), m_orignpos);
		pos.Set(ja.v[3]);
		auto len = pos - m_pos;
		m_pos = pos;
		return len;
	}
	void StateRig(CMatrix pos) {
		m_orignpos = pos;
		CMatrix ja;
		ja.Mul(m_Bone->GetLocalMatrix(), m_orignpos);
		m_pos.Set(ja.v[3]);
		m_orignpos = pos;
	}
	CMatrix m_orignpos;
	Bone* m_Bone = nullptr;
	CVector3 m_pos = CVector3::Zero();
};

