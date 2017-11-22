#include "SystemWindows.hpp"

#include <chrono>

namespace OrbitEngine { namespace System { namespace priv {
	const char* SystemWindows::getName()
	{
		return "Windows";
	}

	long long SystemWindows::currentNano()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
} } }
