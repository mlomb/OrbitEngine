#ifndef GRAPHICS_SHADER_LOADER_HPP
#define GRAPHICS_SHADER_LOADER_HPP

#include <string>

#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine { namespace Graphics {
	/*
		This is the class in charge of loading .oeshader files
	*/
	class ShaderLoader {
	public:
		/*
			This will compile HLSL source code to
			match the actual context Render API
			If the API is Direct3D then the shader
			will only be preprocessed
		*/
		static std::string CrossCompileHLSL(const std::string hlslSource, ShaderType shaderType, std::string entryPoint = "main");

		static Shader* FromFile(const std::string& path);
		static Shader* FromSource(const std::string& source);

		static Shader* Batch2D() { return LoadInternalShader("Batch2D"); };
		static Shader* Vector2D() { return LoadInternalShader("Vector2D"); };
		static Shader* GBuffer() { return LoadInternalShader("GBuffer"); };
		static Shader* DeferredPBR() { return LoadInternalShader("DeferredPBR"); };
		static Shader* IntegrateBRDF() { return LoadInternalShader("IntegrateBRDF"); };
	private:
		static Shader* LoadInternalShader(const std::string& name);
		static std::string LoadFile(const std::string& path);

		~ShaderLoader() {};
		ShaderLoader() {};
	};
} }

#endif