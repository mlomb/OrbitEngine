#ifndef GRAPHICS_SHADER_COMPILER_HPP
#define GRAPHICS_SHADER_COMPILER_HPP

#include <string>

#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine { namespace Graphics {

	struct ShaderCompilationResult {
		std::string source;
		ShaderReflection reflection;
	};

	class ShaderCompiler {
	public:
		/*
			This will compile and preprocess
			HLSL source code to match the actual context Render API
			If the API is Direct3D then the shader
			will only be preprocessed
		*/
		static ShaderCompilationResult CrossCompileHLSL(const std::string hlslSource, ShaderType shaderType, std::string entryPoint = "main");
	private:

		ShaderCompiler() {};
	};
} }

#endif