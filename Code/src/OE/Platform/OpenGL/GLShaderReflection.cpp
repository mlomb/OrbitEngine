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
		OE_CHECK_GL(glGetProgramiv(id, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBlocksCount));

		for (int i = 0; i < uniformBlocksCount; i++) {
			// Get block name
			OE_CHECK_GL(glGetActiveUniformBlockName(id, i, maxLength - 1, NULL, name));

			// Get block binding
			GLint binding = -1;
			OE_CHECK_GL(glGetActiveUniformBlockiv(id, i, GL_UNIFORM_BLOCK_BINDING, &binding));
			if (binding < 0 || binding > GL_MAX_UNIFORM_BUFFER_BINDINGS) {
				// Binding is invalid, so we're going to assume that binding = i
				binding = i;
			}

			/*
				TODO: Temporal fix
				For some reason, glGetActiveUniformBlockiv is
				returning 0 for different uniform blocks
			*/
			binding = uniformBlocksCount - 1 - i;

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