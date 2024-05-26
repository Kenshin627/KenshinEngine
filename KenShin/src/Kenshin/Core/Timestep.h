#pragma once
#include "Kenshin/Core.h"
#include <cstdint>

namespace Kenshin {
	class KS_API Timestep
	{
	public:
		Timestep(float time) :m_Time(time) {}
		operator float() { return m_Time; }
		uint32_t GetSeconds() const { return m_Time; }
		uint32_t GetMillSeconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}