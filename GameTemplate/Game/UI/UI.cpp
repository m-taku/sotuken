#include "stdafx.h"
#include "UI.h"

namespace {
	const CVector3 HP_POSITION = CVector3{ -500.0f,500.0f,0.0f };
	const CVector3 STAMINA_POSITION = CVector3{ -500.0f,400.0f,0.0f };
	const CVector3 TIMER_POSITION = CVector3{ -700.0f,400.0f,0.0f };
	const CVector2 HP_TEX_WH = CVector2{ 500.0f,10.0f };
	const CVector2 STAMINA_TEX_WH = HP_TEX_WH;
	const CVector2 TIMER_TEX_WH = CVector2{ 400.0f,400.0f };;
	const CVector2 TIMERMIN_TEX_WH = TIMER_TEX_WH;
	const CVector2 HP_PIVOT = CVector2{ -1.0f,1.0f };
	const CVector2 STAMINA_PIVOT = CVector2{ -1.0f,1.0f };
	const CVector2 TIMER_PIVOT = CVector2{ 0.0f,0.0f };
	const CVector2 TIMER_MIN_PIVOT = CVector2{ 0.0f,-1.0f };
	const int Player_HP_MAX= 150;
	const int Player_STAMINA_MAX = 150;
}


UI::UI()
{
}


UI::~UI()
{
}

void UI::Init(int MaxHP, int MaxStamina, int MaxQuestTime)
{
	MAX_HP = MaxHP;
	MAX_STAMINA = MaxStamina;
	MAX_QUESTTIME = MaxQuestTime;
}

bool UI::Start()
{
	m_HPTex.CreateFromDDSTextureFromFile(L"Assets/sprite/HPVer_Naka.dds");
	m_HPSprite.Init(m_HPTex, HP_TEX_WH.x, HP_TEX_WH.y);
	m_HPPosition = HP_POSITION;


	m_HPTex_Waku.CreateFromDDSTextureFromFile(L"Assets/sprite/HPVer_Waku.dds");
	m_HPSprite_Waku.Init(m_HPTex_Waku, HP_TEX_WH.x, HP_TEX_WH.y);
	m_HPPosition = HP_POSITION;

	m_StaminaTex.CreateFromDDSTextureFromFile(L"Assets/sprite/StaminaVer_Naka.dds");
	m_StaminaSprite.Init(m_StaminaTex, STAMINA_TEX_WH.x, STAMINA_TEX_WH.y);
	m_StaminaPosition = STAMINA_POSITION;

	m_StaminaTex_Waku.CreateFromDDSTextureFromFile(L"Assets/sprite/StaminaVer_Waku.dds");
	m_StaminaSprite_Waku.Init(m_StaminaTex_Waku, STAMINA_TEX_WH.x, STAMINA_TEX_WH.y);
	m_HPPosition = HP_POSITION;
	

	m_TimerTex.CreateFromDDSTextureFromFile(L"Assets/sprite/Timer_Waku.dds");
	m_TimerSprite.Init(m_TimerTex, TIMER_TEX_WH.x, TIMER_TEX_WH.y);
	m_TimerPosition = TIMER_POSITION;

	m_TimerMinTex.CreateFromDDSTextureFromFile(L"Assets/sprite/Timer_Hari.dds");
	m_TimerMinSprite.Init(m_TimerMinTex, TIMERMIN_TEX_WH.x, TIMERMIN_TEX_WH.y);
	m_TimerMinPosition = TIMER_POSITION;



	m_HPSprite.Update(m_HPPosition, m_rot, m_HPTexScale, HP_PIVOT);
	m_HPSprite_Waku.Update(m_HPPosition, m_rot, m_HPTexScale, HP_PIVOT);
	m_StaminaSprite.Update(m_StaminaPosition, m_rot, m_StaminaTexScale, STAMINA_PIVOT);
	m_StaminaSprite_Waku.Update(m_HPPosition, m_rot, m_HPTexScale, HP_PIVOT);
	m_TimerSprite.Update(m_TimerPosition, m_rot, m_TimerTexScale, TIMER_PIVOT);
	m_TimerMinSprite.Update(m_TimerMinPosition, m_MinRot, m_TimerMinTexScale, TIMER_PIVOT);
	return true;
}

void UI::Update()
{
	switch (m_map)
	{
	case UI::IsTown:
		switch (m_haveQuest)
		{
		case UI::haveQuest:
			m_TimerSprite.Update(m_TimerPosition, m_rot, m_TimerTexScale, TIMER_PIVOT);
			break;
		default:
			break;
		}
		break;
	case UI::IsQuest:
		m_HPTexScale.x = 1.0f*((float)HP / max(1.0f, (float)Player_HP_MAX));
		m_StaminaTexScale.x = 1.0f*((float)Stamina / max(1.0f, (float)Player_STAMINA_MAX));
		m_MinRot.SetRotationDeg(CVector3::AxisZ(), -360.0f*((float)Time / max((float)MAX_QUESTTIME, 1.0f)));
		m_HPSprite.Update(m_HPPosition,m_rot,m_HPTexScale, HP_PIVOT);
		m_HPTexScale.x = 1.0f*((float)MAX_HP / max(1.0f, (float)Player_HP_MAX));
		m_HPSprite_Waku.Update(m_HPPosition, m_rot, m_HPTexScale, HP_PIVOT);
		m_StaminaSprite.Update(m_StaminaPosition, m_rot, m_StaminaTexScale, STAMINA_PIVOT);
		m_StaminaTexScale.x = 1.0f*((float)MAX_STAMINA / max(1.0f, (float)Player_STAMINA_MAX));
		m_StaminaSprite_Waku.Update(m_StaminaPosition, m_rot, m_StaminaTexScale, STAMINA_PIVOT);
		m_TimerSprite.Update(m_TimerPosition, m_rot, m_TimerTexScale, TIMER_PIVOT);
		m_TimerMinSprite.Update(m_TimerMinPosition, m_MinRot, m_TimerMinTexScale, TIMER_PIVOT);
		break;
	default:
		break;
	}

}

void UI::UIDraw()
{
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	switch (m_map)
	{
	case UI::IsTown:
		switch (m_haveQuest)
		{
		case UI::haveQuest:
			m_TimerSprite.Draw(deviceContext);
			break;
		default:
			break;
		}
		break;
	case UI::IsQuest:
		m_HPSprite.Draw(deviceContext);
		m_HPSprite_Waku.Draw(deviceContext);
		m_StaminaSprite.Draw(deviceContext);
		m_StaminaSprite_Waku.Draw(deviceContext);
		m_TimerSprite.Draw(deviceContext);
		m_TimerMinSprite.Draw(deviceContext);
		break;
	default:
		break;
	}

}
