#ifndef GRAPHICS_VKFRAMEBUFFER_HPP
#define GRAPHICS_VKFRAMEBUFFER_HPP

#include "OE/Platform/Vulkan/Vulkan.hpp"

#include "OE/Graphics/API/FrameBuffer.hpp"

namespace OrbitEngine {	namespace Graphics {
	class VKFrameBuffer : public FrameBuffer {
	public:
		VKFrameBuffer();
		~VKFrameBuffer();
		
	private:

	};
} }

#endif