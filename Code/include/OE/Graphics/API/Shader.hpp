#ifndef GRAPHICS_SHADER_HPP
#define GRAPHICS_SHADER_HPP

#include <string>
#include <vector>

namespace OrbitEngine {	namespace Graphics {
	enum ShaderType {
		UNKNOWN = 0,
		VERTEX,
		FRAGMENT, // or PIXEL
		GEOMETRY,
		TESS_CONTROL,
		TESS_EVAL
	};

	class Shader {
	public:
		~Shader();

		static Shader* Create();

		void attachFromFile(ShaderType type, const std::string& path, bool binary = false);

		virtual void attachFromSource(ShaderType type, const std::string& source) = 0;
		virtual void attachFromBinary(ShaderType type, const std::vector<char>& binary) = 0;
		virtual void finalize() = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

	protected:
		Shader();

	public:
		static Shader* LoadInternalShader(const char* name);

		static Shader* Batch2D() { return LoadInternalShader("Batch2D"); };
		static Shader* Vector2D() { return LoadInternalShader("Vector2D"); };
		static Shader* GBuffer() { return LoadInternalShader("GBuffer"); };
		static Shader* DeferredPBR() { return LoadInternalShader("DeferredPBR"); };
		static Shader* Skybox() { return LoadInternalShader("Skybox"); };
		static Shader* IntegrateBRDF() { return LoadInternalShader("IntegrateBRDF"); };
		static Shader* PrefilterEnviromentPBR() { return LoadInternalShader("PrefilterEnvMap"); };
		static Shader* HDRToCubemap() { return LoadInternalShader("HdrToCubemap"); };
	};
} }

#endif