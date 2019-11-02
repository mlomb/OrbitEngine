#ifndef GRAPHICS_SHADER_REFLECTION_HPP
#define GRAPHICS_SHADER_REFLECTION_HPP

#include <vector>
#include <string>

namespace OrbitEngine {	namespace Graphics {

	enum class ShaderUniformType {
		UNKNOWN,
		BOOL,
		INT,
		UINT,
		HALF,
		FLOAT,
		DOUBLE,
	};

	struct ShaderUniform {
		std::string name;
		ShaderUniformType type;
		size_t size;
		size_t offset;
		// rows, cols
		unsigned int dimensions[2] = { 1, 1 };
	};

	struct ShaderBuffer {
		std::string name;
		size_t size;
		size_t padding;
		unsigned int slot;
		std::vector<ShaderUniform> uniforms;
	};

	struct ShaderAttribute {
		std::string name;
	};

	struct ShaderReflection {
		std::vector<ShaderBuffer> buffers;
		std::vector<ShaderAttribute> inputAttributes, outputAttributes;
	};
} }

#endif