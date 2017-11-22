#include "SystemEmscripten.hpp"

#include <chrono>

namespace OrbitEngine { namespace System { namespace priv {
	const char* SystemEmscripten::getName()
	{
		return "Emscripten";
	}

	long long SystemEmscripten::currentNano()
	{
		// TODO Change?
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
} } }
