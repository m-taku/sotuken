#pragma once
class TestSave : public Noncopyable
{
public:
	TestSave();
	virtual	~TestSave();
	virtual	bool Seve() = 0;
	virtual	void Load() = 0;
	template<class T>
	T* GetData()
	{
		return (T*)data;
	}
protected:
	//ここからセーブデータ用のクラス
	void* data;
};

