#ifndef GRAPHICS_SHADER_MANAGER_HPP
#define GRAPHICS_SHADER_MANAGER_HPP

#include <Xsc/Xsc.h>

#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine { namespace Graphics {
	class ShaderManager {
	public:

		/*
			This will compile arbitrary HLSL code to
			match the actual context Render API
		*/
		static std::string CrossCompileHLSL(const std::string hlslSource, ShaderType shaderType, std::string entryPoint = "main");

	private:

		static Xsc::ShaderTarget ShaderToXSCShader(ShaderType shader);
		static Xsc::OutputShaderVersion OutputShaderVersionFromCurrentContext();

		~ShaderManager() {};
		ShaderManager() {};
	};
} }

#endif