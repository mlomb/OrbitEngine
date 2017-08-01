#include "OE/System/System.hpp"

#if OE_WINDOWS
#elif OE_ANDROID
	#include "OE/Platform/Android/SystemAndroid.hpp"
	typedef OrbitEngine::Application::priv::SystemAndroid SystemImplType;
#elif OE_EMSCRIPTEN
#elif OE_UNIX
#endif

namespace OrbitEngine { namespace System {
	System::System()
#if OE_ANDROID
		: Pimpl(new SystemImplType())
#else
		: Pimpl(0)
#endif
	{
	}

	System::~System()
	{
	}
	const char * System::getName()
	{
		return nullptr;
	}
	long long System::currentMillis()
	{
		return 42;
	}
} }