#ifndef SYSTEM_SYSTEM_WINDOWS_HPP
#define SYSTEM_SYSTEM_WINDOWS_HPP

#include "OE/System/SystemImpl.hpp"

namespace OrbitEngine { namespace System { namespace priv {

	class SystemWindows : public SystemImpl {
	public:
		SystemWindows() {};
		~SystemWindows() {};

		std::vector<MonitorDesc> getMonitors() override;
		const char* getName() override;
		long long currentNano() override;

	protected:

	};
} } }

#endif