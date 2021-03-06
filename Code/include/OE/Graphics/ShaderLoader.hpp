#ifndef GRAPHICS_SHADER_LOADER_HPP
#define GRAPHICS_SHADER_LOADER_HPP

#include <string>

#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine { namespace Graphics {

	class ShaderLoader {
	public:
		static Shader* LoadRawShader(const std::string& path);
		static ShaderSources LoadShaderSources(const std::string& path);
		static std::string ResolveShaderFile(const std::string& path);

		static Shader* Batch2D() { return LoadInternalShader("Batch2D"); };
		static Shader* Vector2D() { return LoadInternalShader("Vector2D"); };
		static Shader* Text2D() { return LoadInternalShader("Text2D"); };
		static Shader* IntegrateBRDF() { return LoadInternalShader("IntegrateBRDF"); };
		static Shader* Skybox() { return LoadInternalShader("Skybox"); };
		static Shader* PrefilterEnvMap() { return LoadInternalShader("PrefilterEnvMap"); };
		static Shader* UI() { return LoadInternalShader("UI"); };
	private:
		static Shader* LoadInternalShader(const std::string& name);

		ShaderLoader() {};
	};
} }

#endif