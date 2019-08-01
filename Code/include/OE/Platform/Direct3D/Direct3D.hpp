#ifndef D3D_HPP
#define D3D_HPP

#include "OE/Config.hpp"

#if OE_D3D
	#include "OE/Platform/Windows/Windows.hpp"

	// Source: https://stackoverflow.com/a/48712518
	// Only undefine, if DXGIType.h has not been included yet
	#pragma region Undefine Windows Macros
		#ifndef __dxgitype_h__
			#undef DXGI_STATUS_OCCLUDED
			#undef DXGI_STATUS_CLIPPED
			#undef DXGI_STATUS_NO_REDIRECTION
			#undef DXGI_STATUS_NO_DESKTOP_ACCESS
			#undef DXGI_STATUS_GRAPHICS_VIDPN_SOURCE_IN_USE
			#undef DXGI_STATUS_MODE_CHANGED
			#undef DXGI_STATUS_MODE_CHANGE_IN_PROGRESS
			#undef DXGI_ERROR_INVALID_CALL
			#undef DXGI_ERROR_NOT_FOUND
			#undef DXGI_ERROR_MORE_DATA
			#undef DXGI_ERROR_UNSUPPORTED
			#undef DXGI_ERROR_DEVICE_REMOVED
			#undef DXGI_ERROR_DEVICE_HUNG
			#undef DXGI_ERROR_DEVICE_RESET
			#undef DXGI_ERROR_WAS_STILL_DRAWING
			#undef DXGI_ERROR_FRAME_STATISTICS_DISJOINT
			#undef DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE
			#undef DXGI_ERROR_DRIVER_INTERNAL_ERROR
			#undef DXGI_ERROR_NONEXCLUSIVE
			#undef DXGI_ERROR_NOT_CURRENTLY_AVAILABLE
			#undef DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED
			#undef DXGI_ERROR_REMOTE_OUTOFMEMORY
			#undef D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
			#undef D3D11_ERROR_FILE_NOT_FOUND
			#undef D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS
			#undef D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD
			#undef D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
			#undef D3D10_ERROR_FILE_NOT_FOUND
		#endif
	#pragma endregion

	#include <D3D11.h>
	#include "D3DCompiler.h"

	#define OE_D3D_RELEASE(x) \
	{ \
		if(x) { \
			x->Release(); \
			x = 0; \
		} \
	}
#endif

#endif