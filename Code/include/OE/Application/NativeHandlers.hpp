#ifndef APPLICATION_NATIVE_HANDLERS_HPP
#define APPLICATION_NATIVE_HANDLERS_HPP

#include "OE/Config.hpp"

#if OE_WINDOWS
	#include "OE/Platform/Windows/Windows.hpp"
#elif OE_ANDROID
	#include "OE/Platform/Android/Android.hpp"
#elif OE_LINUX
	#include "OE/Platform/X11/X11.hpp"
#endif

namespace OrbitEngine {	namespace Application { 

#if OE_WINDOWS
	typedef HWND WindowNativeHandle;
	typedef HDC DisplayNativeHandle;
#elif OE_ANDROID
	typedef ANativeWindow* WindowNativeHandle; // ANativeWindow*
	typedef void* DisplayNativeHandle;
#elif OE_LINUX
	typedef ::Window WindowNativeHandle;
	typedef ::Display* DisplayNativeHandle;
#else
	typedef int WindowNativeHandle;
	typedef int DisplayNativeHandle;
#endif

} }

#endif