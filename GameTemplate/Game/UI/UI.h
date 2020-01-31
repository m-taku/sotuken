#pragma once
class UI :public IGameObject
{
public:
	UI();
	~UI();
	void Init(int MaxHP, int MaxStamina,int MaxQuestTime);
	bool Start();
	void Update();
	void UIDraw();

	inline void SetHP(int hp)
	{
		HP = hp;
	}

	inline void SetStamina(int stamina)
	{
		Stamina = stamina;
	}

	inline void SetTimer(int Min)
	{
		Time = Min;
	}
private:

	int MAX_HP = 150;
	int MAX_STAMINA = 150;
	int MAX_QUESTTIME = 50;

	int HP = 150;
	int Stamina = 150;
	int Time = 50;

	Sprite m_HPSprite;
	Sprite m_StaminaSprite;
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
	ShaderResourceView m_TimerTex;
	ShaderResourceView m_TimerMinTex;
};

