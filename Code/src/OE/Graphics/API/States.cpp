#include "OE/Graphics/API/Pipeline.hpp"

#include "OE/Application/Context.hpp"

#if OE_VULKAN
	#include "OE/Platform/Vulkan/VKPipeline.hpp"
#endif

namespace OrbitEngine {	namespace Graphics {
	void Pipeline::setShader(Shader* shader)
	{
		p_Shader = shader;
	}

	Pipeline* Pipeline::Create() {
		return 0;
		//return new VKPipeline();
	}
} }