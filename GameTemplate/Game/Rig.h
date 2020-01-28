#pragma once
class Rig : public Noncopyable
{
public:
	Rig();
	~Rig();
	void SetBoon(SkinModel& skinmode, const wchar_t* name)
	{
		auto na = skinmode.FindBone(name);
		na->GetWorldMatrix();
	}
	Bone* m_;
};

