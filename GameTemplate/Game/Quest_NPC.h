#pragma once
class QuestManager;
class Text_Box;
#include"NPCBase.h"
class Quest_NPC : public NPCBase
{
public:
	Quest_NPC();
	~Quest_NPC();
	bool Init();

	//外部から変更する分
	enum TalkState
	{
		QuestSelect,
		QuestCancel
	};
	void SetTalkState(TalkState state)
	{
		m_TalkState = state;
		m_TalkChange = TalkEnd;
	}
	void Talk();
	void Draw();
private:
	//自分限定
	enum TalkChange
	{
		TalkStart,
		TalkWate,
		PrintQuest,
		TalkEnd
	};
	bool isupdate = false;
	CharacterController m_characon;		//キャラコン
	SkinModel m_model;
	SkinModel m_bikkuri;
	Text_Box* m_Text[2] = { nullptr };
	TalkChange m_TalkChange = TalkStart;
	TalkState m_TalkState = QuestSelect;
	QuestManager* m_QuestManager = nullptr;
	CVector3 m_movespeed = CVector3::Zero();

	//
	float m_fallSpeed = 0.0f;
	float m_addGravityTime = 0.0f;
};

