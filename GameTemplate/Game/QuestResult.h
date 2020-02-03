#pragma once
#include"Text_Box/Text_Box.h"
class QuestResult : public IGameObject
{
public:
	QuestResult();
	~QuestResult();
	bool Start();
	void Update();
	void SetClear(bool frag)
	{
		IsClear = frag;
	}
private:
	bool IsClear = false;
	float m_nowtime = 0.0f;
	Text_Box* m_text[4] = { nullptr };
};

