#include "OE/Config.hpp"

#include "OE/System/System.hpp"

#if OE_WINDOWS
	#include "OE/Platform/Windows/SystemWindows.hpp"
	typedef OrbitEngine::System::priv::SystemWindows SystemImplType;
#elif OE_ANDROID
	#include "OE/Platform/Android/SystemAndroid.hpp"
	typedef OrbitEngine::System::priv::SystemAndroid SystemImplType;
#elif OE_EMSCRIPTEN
	#include "OE/Platform/Emscripten/SystemEmscripten.hpp"
	typedef OrbitEngine::System::priv::SystemEmscripten SystemImplType;
#elif OE_UNIX
	#include "OE/Platform/X11/SystemLinux.hpp"
	typedef OrbitEngine::System::priv::SystemLinux SystemImplType;
#endif

namespace OrbitEngine { namespace System {
	System::System()
		: Pimpl(new ::SystemImplType())
	{
	}

	System::~System()
	{
	}

	const char* System::getName()
	{
		return p_Pimpl->getName();
	}

	long long System::currentNano()
	{
		return p_Pimpl->currentNano();
	}
} }