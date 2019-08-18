#include "OE/Misc/Ticker.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/System/System.hpp"

namespace OrbitEngine { namespace Misc {
	Ticker::Ticker()
	{
		m_TicksTimer = -1;
		m_FPSTimer = -1;

		m_FrameDelta = 0;
		m_FPSCounter = 0;
		m_FPS = 0;

		m_TotalTicks = 0;
	}

	void Ticker::tick()
	{
		m_TotalTicks++;

		m_Now = System::System::Get()->currentNano();
		m_FrameDelta = (m_Now - m_TicksTimer) / (float)ONE_SECOND_IN_NANO;
		if (m_TicksTimer == -1) m_FrameDelta = 0;
		m_TicksTimer = m_Now;
		m_FPSCounter++;

		if (m_FPSTimer == -1)
			m_FPSTimer = m_Now;
		else if (m_Now - m_FPSTimer > ONE_SECOND_IN_NANO) {
			m_FPS = m_FPSCounter;

			OE_LOG_DEBUG(std::to_string(m_FPS) + "fps (" + std::to_string(getFrameDeltaMS()) + ")");

			m_FPSCounter = 0;
			do {
				m_FPSTimer += ONE_SECOND_IN_NANO;
			} while (m_Now - m_FPSTimer > ONE_SECOND_IN_NANO);
		}
	}

	long long Ticker::getTimeSinceLastTick() {
		return System::System::Get()->currentNano() - m_TicksTimer;
	}
} }