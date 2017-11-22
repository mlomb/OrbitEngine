#include "SystemLinux.hpp"

#include <chrono>

namespace OrbitEngine { namespace System { namespace priv {
	SystemLinux::SystemLinux()
	{
	}
	const char* SystemLinux::getName()
	{
		return "Linux";
	}

	long long SystemLinux::currentNano()
	{
		// TODO Change?
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
} } }
