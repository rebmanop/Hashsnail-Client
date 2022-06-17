#pragma once
#include<chrono>

class Timer
{
public:
	Timer() = default;
	
	void Start() 
	{
		m_Start = std::chrono::high_resolution_clock::now();
	}

	void End()
	{
		m_End = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = m_End - m_Start;
		m_TimePassed = duration.count();

	}

	float GetTime(const char* format) const
	{
		if (format == "s")
			return m_TimePassed;

		else if (format == "ms")
			return m_TimePassed * 1000.0f;

		else
			return m_TimePassed;
	}

	~Timer() = default;

private:
	std::chrono::time_point<std::chrono::steady_clock> m_Start, m_End;
	float m_TimePassed = -1;
};