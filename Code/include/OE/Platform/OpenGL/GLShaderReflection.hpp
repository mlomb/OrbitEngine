#ifndef GRAPHICS_GLSHADER_REFLECTION_HPP
#define GRAPHICS_GLSHADER_REFLECTION_HPP

#include "OE/Graphics/API/ShaderReflection.hpp"
#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine {	namespace Graphics {
	class GLShaderReflection : public ShaderReflection {
	public:
		GLShaderReflection();
		~GLShaderReflection();

		void reflect(GLint id);

	private:
		void reflectUniformBlocks(GLint id);

	};
} }

#endif