#pragma once
class UI :public IGameObject
{
public:
	UI();
	~UI();
	enum Map
	{
		IsTown,
		IsQuest		
	};
	enum HaveQuest
	{
		NothaveQuest,
		haveQuest
	};
	void Init(int MaxHP, int MaxStamina,int MaxQuestTime);
	bool Start();
	void Update();
	void UIDraw();

	inline void SetHP(int hp)
	{
		HP = hp;
	}

	inline void SetStamina(float stamina)
	{
		Stamina = (int)stamina;
	}

	inline void SetTimer(int Min)
	{
		Time = Min;
	}
	inline void SetIsMap(Map map)
	{
		m_map = map;
		m_haveQuest = NothaveQuest;
	}
	inline void SetHaveQuest(HaveQuest map)
	{
		m_haveQuest = map;
	}
	
private:

	int MAX_HP = 150;
	int MAX_STAMINA = 150;
	int MAX_QUESTTIME = 50;

	int HP = 150;
	int Stamina = 150;
	int Time = 50;
	Map m_map = IsTown;
	HaveQuest m_haveQuest = NothaveQuest;

	Sprite m_HPSprite;
	Sprite m_StaminaSprite;
	Sprite m_HPSprite_Waku;
	Sprite m_StaminaSprite_Waku;
	Sprite m_TimerMinSprite;
	Sprite m_TimerSprite;


	CVector3 m_HPPosition = CVector3::Zero();
	CVector3 m_StaminaPosition = CVector3::Zero();
	CVector3 m_TimerMinPosition = CVector3::Zero();
	CVector3 m_TimerPosition = CVector3::Zero();

	CVector3 m_HPTexScale = CVector3::One();
	CVector3 m_StaminaTexScale = CVector3::One();
	CVector3 m_TimerMinTexScale = CVector3::One();
	CVector3 m_TimerTexScale = CVector3::One();

	CQuaternion m_rot = CQuaternion::Identity();
	CQuaternion m_MinRot = CQuaternion::Identity();

	ShaderResourceView m_HPTex;
	ShaderResourceView m_StaminaTex;
	ShaderResourceView m_HPTex_Waku;
	ShaderResourceView m_StaminaTex_Waku;
	ShaderResourceView m_TimerTex;
	ShaderResourceView m_TimerMinTex;
};

