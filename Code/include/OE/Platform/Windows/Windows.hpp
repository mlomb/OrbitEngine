#ifndef WINDOWS_HPP
#define WINDOWS_HPP

#include "OE/Config.hpp"

#if OE_WINDOWS
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <windowsx.h>
	#include <dwmapi.h>
	#pragma comment(lib, "dwmapi.lib")
#endif

#endif
