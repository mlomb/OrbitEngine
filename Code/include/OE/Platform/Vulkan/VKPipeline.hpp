#ifndef GRAPHICS_VKPIPELINE_HPP
#define GRAPHICS_VKPIPELINE_HPP

#include "OE/Platform/Vulkan/Vulkan.hpp"

#include "OE/Graphics/API/Pipeline.hpp"

namespace OrbitEngine {	namespace Graphics {
	class VKPipeline : public Pipeline {
	public:
		VKPipeline();
		~VKPipeline();

		void flush() override;

	private:

		VkPipeline m_Pipeline;

	};
} }

#endif