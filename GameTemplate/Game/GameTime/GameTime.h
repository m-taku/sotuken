#pragma once
#include "stdafx.h"
//#include<vector>
//#include"Noncopyable/Noncopyable.h"
namespace smEngine {
	class GameTime : public Noncopyable
	{
	public:
		GameTime()
		{

		}
		~GameTime()
		{

		}
		static inline GameTime& Instance()
		{
			static GameTime Instance;
			return Instance;
		}
		void Start()
		{
			::QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
			m_elapsed = 0.0;
		}
		void Stop()
		{
			::QueryPerformanceFrequency((LARGE_INTEGER*)&m_fleam);
			::QueryPerformanceCounter((LARGE_INTEGER*)&m_stop);
			m_elapsed = double(m_stop - m_start) / m_fleam;
			SetFrameDeltaTime((float)m_elapsed);
			//::QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
		}
		float GetFrameTime()
		{
			return m_frameDelta;
		}
		void SetFrameDeltaTime(float deltaTime)
		{
			m_frameDelta = deltaTime;
		}
	private:
		float m_frameDelta = 1.0f / 60.0f;					//�t���[���f���^�^�C��
		LONGLONG m_start = 0;				//�X�^�[�g�������̕b��
		LONGLONG m_stop = 0;				//�X�g�b�v�������̕b��
		LONGLONG m_fleam = 0;				//�X�g�b�v�������̃t���[����
		double m_elapsed = 0.0;				//�o�ߎ���(�P�ʁF�b)
	};
	static GameTime& GetGameTime()
	{
		return GameTime::Instance();
	}
	static float GetFrameDeltaTime()
	{
		return min(GameTime::Instance().GetFrameTime(),1.0f/30.0f);
	}
}


