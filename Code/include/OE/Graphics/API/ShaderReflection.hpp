#ifndef GRAPHICS_SHADER_REFLECTION_HPP
#define GRAPHICS_SHADER_REFLECTION_HPP

#include <vector>
#include <string>

#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine {	namespace Graphics {
	struct ShaderBufferReflection {
		std::string name;
		unsigned int slot;
		ShaderType shaderType;
	};

	class ShaderReflection {
	public:
		virtual ~ShaderReflection();

		std::vector<ShaderBufferReflection> getBuffers(const std::string& name) const;
		std::vector<ShaderBufferReflection> getAllBuffers() const;

		static ShaderReflection* Create();
	protected:
		ShaderReflection();

		void addBuffer(ShaderBufferReflection buffer);

	private:
		std::vector<ShaderBufferReflection> m_Buffers;
	};
} }

#endif