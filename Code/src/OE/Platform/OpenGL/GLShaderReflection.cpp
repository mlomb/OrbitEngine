#include "OE/Platform/OpenGL/GLShaderReflection.hpp"

#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine {	namespace Graphics {
	GLShaderReflection::GLShaderReflection()
	{

	}

	GLShaderReflection::~GLShaderReflection()
	{

	}

	void GLShaderReflection::reflect(GLint id)
	{
		reflectUniformBlocks(id);
	}

	void GLShaderReflection::reflectUniformBlocks(GLint id)
	{
		int uniformBlocksCount;
		const int maxLength = 100;
		char name[maxLength];
		glGetProgramiv(id, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBlocksCount);

		for (int i = 0; i < uniformBlocksCount; i++) {
			// Get block name
			glGetActiveUniformBlockName(id, i, maxLength - 1, NULL, name);

			// Get block binding
			GLint binding;
			glGetActiveUniformBlockiv(id, i, GL_UNIFORM_BLOCK_BINDING, &binding);

			ShaderBufferReflection buffer;
			// It's not necessary in OpenGL
			// If some day is necessary use
			// GL_UNIFORM_BLOCK_REFERENCED_BY_*
			buffer.shaderType = ShaderType::UNKNOWN;
			buffer.name = name;
			buffer.slot = binding;
			addBuffer(buffer);
		}
	}
} }