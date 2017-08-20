#ifndef GRAPHICS_D3DSHADER_REFLECTION_HPP
#define GRAPHICS_D3DSHADER_REFLECTION_HPP

#include "OE/Platform/Direct3D/Direct3D.hpp"
#include "OE/Graphics/API/ShaderReflection.hpp"
#include "OE/Graphics/API/Shader.hpp"

#include <vector>

namespace OrbitEngine {	namespace Graphics {
	class D3DShaderReflection : public ShaderReflection {
	public:
		D3DShaderReflection();
		~D3DShaderReflection();

		void reflect(const std::vector<char>& binary, ShaderType type);

		ID3D11InputLayout* getVertexInputLayout();

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> reflectInputLayout(D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* reflector);
		void reflectBuffers(D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* reflector, ShaderType type);

		ID3D11InputLayout* m_VertexInputLayout;
	};
} }

#endif