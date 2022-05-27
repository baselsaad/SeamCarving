#pragma once
#include <chrono>


class Timer {

public:

	Timer();

	void StartTimer() const;

	__forceinline float ElapsedTimeInMili() const { return Timer::m_ElapsedTime; }

	__forceinline float ElapsedTimeInSecound() const { return Timer::m_ElapsedTime / 1000.0f; }

	void StopTimer() const;

private:
	static const int INVALID = -1;
	mutable float m_ElapsedTime;
	mutable bool m_IsTimerStarted;
	mutable std::chrono::time_point<std::chrono::steady_clock> m_Start;
	mutable std::chrono::time_point<std::chrono::steady_clock> m_End;
	mutable std::chrono::duration<float> m_Duration;
};

