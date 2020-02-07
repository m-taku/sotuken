#pragma once

class Fade
{
public:
	Fade();
	~Fade();
	void Update();
	void SetFadeInSpeed(float time)
	{
		m_inTime = time;
	}
	void SetFadeOutSpeed(float time)
	{
		m_outTime = time;
	}
	void FadeInStart();
	void FadeOutStart();
	bool IsInFade()
	{
		return m_isInFade;
	}


private:
	void Init();
	enum EState {
		enFadeIn,
		enFadeOut,
		enFadeNone
	};
	const float IN_INIT_COLOR = 0.0f;
	const float OUT_INIT_COLOR = 1.0f;
	float m_inTime = 1.0f;
	float m_outTime = 1.0f;
	float m_time;
	bool m_isInFade;
	float m_MulColor = 1.0f;
	EState m_fadeState = enFadeNone;
};

