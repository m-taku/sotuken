#include "stdafx.h"
#include "UI.h"

namespace {
	const CVector3 HP_POSITION = CVector3{ -500.0f,500.0f,0.0f };
	const CVector3 STAMINA_POSITION = CVector3{ -500.0f,400.0f,0.0f };
	const CVector3 TIMER_POSITION = CVector3{ -700.0f,400.0f,0.0f };
	const CVector2 HP_TEX_WH = CVector2{ 500.0f,10.0f };
	const CVector2 STAMINA_TEX_WH = CVector2{ 500.0f,10.0f };
	const CVector2 TIMER_TEX_WH = CVector2{ 200.0f,200.0f };
	const CVector2 TIMERMIN_TEX_WH = CVector2{ 10.0f,100.0f };
	const CVector2 HP_PIVOT = CVector2{ -1.0f,1.0f };
	const CVector2 STAMINA_PIVOT = CVector2{ -1.0f,1.0f };
	const CVector2 TIMER_PIVOT = CVector2{ 0.0f,0.0f };
	const CVector2 TIMER_MIN_PIVOT = CVector2{ 0.0f,-1.0f };
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
	m_HPTex.CreateFromDDSTextureFromFile(L"Assets/sprite/Title.dds");
	m_HPSprite.Init(m_HPTex, HP_TEX_WH.x*((float)HP / max(1.0f, (float)MAX_HP)), HP_TEX_WH.y);
	m_HPPosition = HP_POSITION;


	m_StaminaTex.CreateFromDDSTextureFromFile(L"Assets/sprite/Title.dds");
	m_StaminaSprite.Init(m_StaminaTex, STAMINA_TEX_WH.x*((float)Stamina / max(1.0f, (float)MAX_STAMINA)), STAMINA_TEX_WH.y);
	m_StaminaPosition = STAMINA_POSITION;

	m_TimerTex.CreateFromDDSTextureFromFile(L"Assets/sprite/Title.dds");
	m_TimerSprite.Init(m_TimerTex, TIMER_TEX_WH.x, TIMER_TEX_WH.y);
	m_TimerPosition = TIMER_POSITION;

	m_TimerMinTex.CreateFromDDSTextureFromFile(L"Assets/sprite/Title.dds");
	m_TimerMinSprite.Init(m_TimerMinTex, TIMERMIN_TEX_WH.x, TIMERMIN_TEX_WH.y);
	m_TimerMinPosition = TIMER_POSITION;

	return true;
}

void UI::Update()
{
	m_HPTexScale.x = 1.0f*((float)HP / max(1.0f, (float)MAX_HP));
	m_StaminaTexScale.x = 1.0f*((float)Stamina / max(1.0f, (float)MAX_STAMINA));
	m_MinRot.SetRotationDeg(CVector3::AxisZ(), -360.0f*((float)Time / max((float)MAX_QUESTTIME, 1.0f)));
	m_HPSprite.Update(m_HPPosition, m_rot, m_HPTexScale, HP_PIVOT);
	m_StaminaSprite.Update(m_StaminaPosition, m_rot, m_StaminaTexScale, STAMINA_PIVOT);
	m_TimerSprite.Update(m_TimerPosition, m_rot, m_TimerTexScale, TIMER_PIVOT);
	m_TimerMinSprite.Update(m_TimerMinPosition, m_MinRot, m_TimerMinTexScale, TIMER_MIN_PIVOT);
}

void UI::UIDraw()
{
	ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
	m_HPSprite.Draw(deviceContext);
	m_StaminaSprite.Draw(deviceContext);
	m_TimerSprite.Draw(deviceContext);
	m_TimerMinSprite.Draw(deviceContext);
}
