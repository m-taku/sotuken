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
		float m_frameDelta = 1.0f / 60.0f;					//フレームデルタタイム
		LONGLONG m_start = 0;				//スタートした時の秒数
		LONGLONG m_stop = 0;				//ストップした時の秒数
		LONGLONG m_fleam = 0;				//ストップした時のフレーム数
		double m_elapsed = 0.0;				//経過時間(単位：秒)
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


