#include "OE/Platform/Direct3D/D3DShader.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Platform/Direct3D/D3DShaderReflection.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Graphics {
	D3DShader::D3DShader()
		: m_VertexShader(0), m_PixelShader(0)
	{
	}

	D3DShader::~D3DShader()
	{
		OE_D3D_RELEASE(m_VertexShader);
		OE_D3D_RELEASE(m_PixelShader);
	}

	void D3DShader::attachFromBinary(ShaderType type, const std::vector<char>& binary)
	{
		HRESULT hr;

		switch (type) {
		case ShaderType::VERTEX:
			hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateVertexShader(binary.data(), binary.size(), NULL, &m_VertexShader);
			break;
		case ShaderType::FRAGMENT:
			hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreatePixelShader(binary.data(), binary.size(), NULL, &m_PixelShader);
			break;
		}
		if (FAILED(hr)) {
			OE_LOG_FATAL("Error creating D3D shader!");
		}

		/* Reflect shader */
		D3DShaderReflection* d3dReflection = static_cast<D3DShaderReflection*>(p_Reflection);
		d3dReflection->reflect(binary, type);
	}

	void D3DShader::attachFromSource(ShaderType type, const std::string& source)
	{
		HRESULT hr;

		ID3DBlob* byteCode = 0;
		ID3DBlob* errorBlob = 0;

		hr = D3DCompile(source.c_str(), source.length(), nullptr, nullptr, nullptr, "main", (GetD3DShaderTarget(type) + "_" + D3D_SHADER_FEATURE_VERSION).c_str(), 0, 0, &byteCode, &errorBlob);
		if (FAILED(hr)) {
			std::string s = std::string((char*)errorBlob->GetBufferPointer());
			OE_LOG_FATAL("Error compiling HLSL shader: " + s);
			OE_D3D_RELEASE(errorBlob);
			return;
		}
		
		const char* ptr = (const char*)byteCode->GetBufferPointer();
		std::vector<char> binary(ptr, ptr + byteCode->GetBufferSize());
		OE_D3D_RELEASE(byteCode);

		attachFromBinary(type, binary);

		binary.clear();
	}

	void D3DShader::finalize()
	{
		// Nothing
	}

	void D3DShader::bind() const
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->VSSetShader(m_VertexShader ? m_VertexShader : 0, nullptr, 0);
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->PSSetShader(m_PixelShader ? m_PixelShader : 0, nullptr, 0);
	}

	std::string D3DShader::GetD3DShaderTarget(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::VERTEX: return "vs";
		case ShaderType::FRAGMENT: return "ps";
		case ShaderType::GEOMETRY: return "gs";
		case ShaderType::TESS_CONTROL: return "hs";
		case ShaderType::TESS_EVAL: return "ds";
		default: return "UNKNOWN";
		}
	}
} }