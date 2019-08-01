#ifndef SYSTEM_SYSTEM_HPP
#define SYSTEM_SYSTEM_HPP

#include <vector>

#include "OE/Misc/Pimpl.hpp"
#include "OE/Misc/Singleton.hpp"

#include "OE/Math/Vec4.hpp"

namespace OrbitEngine {	namespace System {
	namespace priv {
		class SystemImpl;
	}

	struct MonitorDesc {
		Math::Vec4i mainArea;
		Math::Vec4i workArea;
		float dpi;
	};

	class System : public Misc::Pimpl<priv::SystemImpl>, public Misc::Singleton<System> {
	public:

		virtual std::vector<MonitorDesc> getMonitors();
		virtual const char* getName();
		virtual long long currentNano();

	private:
		friend class Misc::Singleton<System>;

		System();
		~System();
	};
} }

#endif