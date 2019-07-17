#ifndef GRAPHICS_SHADER_PASS_HPP
#define GRAPHICS_SHADER_PASS_HPP

#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine { namespace Graphics {

	class ShaderPass {
	public:
		ShaderPass(Shader* shader);

	private:
		Shader* m_Shader;
	};
} }

#endif