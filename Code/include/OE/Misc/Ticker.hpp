#ifndef MISC_TICKER_HPP
#define MISC_TICKER_HPP

#define ONE_SECOND_IN_NANO 1000000000

namespace OrbitEngine { namespace Misc {
	class Ticker {
	public:
		Ticker();

		void tick();

		double getFrameDelta() const { return m_FrameDelta; }
		double getFrameDeltaMS() const { return m_FrameDelta * ONE_SECOND_IN_NANO / 1000000; }
		unsigned short getFPS() const { return m_FPS; }
		unsigned long getTotalTicks() const { return m_TotalTicks; }

		long long getTimeSinceLastTick() const;
		long long getTimeSinceStart() const;
	private:
		long long m_Start;
		long long m_Now, m_TicksTimer, m_FPSTimer;
		double m_FrameDelta;
		unsigned short m_FPSCounter, m_FPS;
		unsigned long m_TotalTicks;
	};
} }

#endif