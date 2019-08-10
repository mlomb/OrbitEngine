#ifndef GRAPHICS_D3DSHADER_HPP
#define GRAPHICS_D3DSHADER_HPP

#include "OE/Platform/Direct3D/Direct3D.hpp"
#include "OE/Graphics/API/Shader.hpp"

#define D3D_SHADER_FEATURE_VERSION "5_0"

namespace OrbitEngine {	namespace Graphics {
	class D3DShader : public Shader {
	public:
		D3DShader();
		~D3DShader();

		void attachFromBinary(ShaderType type, const std::vector<char>& binary) override;
		void attachFromSource(ShaderType type, const std::string& source) override;
		void finalize() override;
		void bind() const override;

		static std::string GetD3DShaderTarget(ShaderType type);
	private:
		ID3D11VertexShader* m_VertexShader;
		ID3D11PixelShader* m_PixelShader;
	};
} }

#endif