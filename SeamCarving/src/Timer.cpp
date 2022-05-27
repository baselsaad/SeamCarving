#include "Timer.h"
#include <iostream>



Timer::Timer() : m_IsTimerStarted(false) {}

void Timer::StartTimer() const
{
	m_Start = std::chrono::high_resolution_clock::now();
	m_IsTimerStarted = true;
}

void Timer::StopTimer() const
{
	m_End = std::chrono::high_resolution_clock::now();

	if (m_IsTimerStarted) {
		m_Duration = m_End - m_Start;
		m_ElapsedTime = m_Duration.count() * 1000.0f;
		m_IsTimerStarted = false;
	}
	else {
		// throw exception
		m_ElapsedTime = INVALID;
	}

}

