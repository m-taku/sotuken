#include "stdafx.h"
#include "Fade.h"


Fade::Fade()
{
}


Fade::~Fade()
{
}

void Fade::Update()
{
	switch (m_fadeState)
	{
	case enFadeIn:
		m_MulColor = min(1.0f, m_time / m_inTime);
		if (m_time >= m_inTime)
		{
			Init();
			break;
		}
		m_time += 1.0f*GetGameTime().GetFrameTime();
		break;
	case enFadeOut:
		m_MulColor = 1.0f - min(1.0f, m_time / m_inTime);
		if (m_time >= m_outTime)
		{
			Init();
			break;
		}
		m_time += 1.0f*GetGameTime().GetFrameTime();
		break;
	case enFadeNone:
		break;
	default:
		break;
	}

	g_graphicsEngine->GetPostEffect()->SetMulColor(m_MulColor);
}

void Fade::FadeInStart()
{
	if (m_fadeState == enFadeNone)
	{
		m_fadeState = enFadeIn;
		m_time = 0.0f;
		m_MulColor = IN_INIT_COLOR;
		m_isInFade = true;
	}
}

void Fade::FadeOutStart()
{
	if (m_fadeState == enFadeNone)
	{
		m_fadeState = enFadeOut;
		m_time = 0.0f;
		m_MulColor = OUT_INIT_COLOR;
		m_isInFade = true;
	}
}

void Fade::Init()
{
	m_fadeState = enFadeNone;
	m_time = 0.0f;
	m_isInFade = false;
}
