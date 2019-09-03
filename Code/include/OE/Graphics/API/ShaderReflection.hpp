#ifndef GRAPHICS_SHADER_REFLECTION_HPP
#define GRAPHICS_SHADER_REFLECTION_HPP

#include <vector>
#include <string>

namespace OrbitEngine {	namespace Graphics {
	struct ShaderUniform {
		std::string name;
		size_t size;
		size_t offset;
	};

	struct ShaderBuffer {
		std::string name;
		size_t size;
		size_t padding;
		unsigned int slot;
		std::vector<ShaderUniform> uniforms;
	};

	struct ShaderReflection {
		std::vector<ShaderBuffer> buffers;
		std::vector<ShaderUniform> uniforms; // global uniforms
	};
} }

#endif