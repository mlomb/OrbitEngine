#ifndef VULKAN_HPP
#define VULKAN_HPP

#include "OE/Config.hpp"

#if OE_VULKAN
	#if OE_WINDOWS
		#include "OE/Platform/Windows/Windows.hpp"
		#define VK_USE_PLATFORM_WIN32_KHR
	#endif
	#include <vulkan/vulkan.h>
#endif

#endif