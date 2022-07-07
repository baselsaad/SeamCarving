#include "Timer.h"
#include <iostream>



Timer::Timer()
	: m_IsTimerStarted(false)
	, m_ElapsedTime(0.0f)
	, m_Duration(0.0f)
{
}

void Timer::StartTimer() const
{
	m_Start = std::chrono::high_resolution_clock::now();
	m_IsTimerStarted = true;
}

void Timer::StopTimer() const
{
	m_End = std::chrono::high_resolution_clock::now();

	if (m_IsTimerStarted)
	{
		m_Duration = m_End - m_Start;
		m_ElapsedTime = m_Duration.count() * 1000.0f;
		m_IsTimerStarted = false;
	}
	else
	{
		m_ElapsedTime = INVALID;
	}

}

