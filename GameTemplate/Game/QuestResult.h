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
	bool m_furag = true;
	Text_Box* m_text[1] = { nullptr };
};

