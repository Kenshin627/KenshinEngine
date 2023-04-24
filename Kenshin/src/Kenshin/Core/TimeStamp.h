#pragma once

namespace Kenshin
{
	class TimeStamp
	{
	public:
		TimeStamp(float time = 0.0f) :m_Time(time) {}
		operator float() const { return m_Time; };
		float GetSeconds() const { return m_Time; }
		float GetmileSeconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}


