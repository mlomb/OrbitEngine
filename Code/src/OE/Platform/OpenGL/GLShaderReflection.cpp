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
		OE_CHECK_GL(glGetProgramiv(id, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBlocksCount));

		const int maxLength = 100;
		char name[maxLength];

		for (int i = 0; i < uniformBlocksCount; i++) {
			// Get block name
			OE_CHECK_GL(glGetActiveUniformBlockName(id, i, maxLength - 1, NULL, name));

			ShaderBufferReflection buffer;
			buffer.name = std::string(name);
			buffer.slot = i;

			// It's not necessary in OpenGL
			// If some day is necessary use
			// glGetActiveUniformBlock with GL_UNIFORM_BLOCK_REFERENCED_BY_*
			buffer.shaderType = ShaderType::UNKNOWN;

			addBuffer(buffer);
		}
	}
} }