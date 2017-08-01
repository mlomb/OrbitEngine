#include "OE/Platform/Direct3D/D3DShader.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Graphics {
	D3DShader::D3DShader()
		: m_InputLayout(0),
		m_VertexShader(0),
		m_PixelShader(0)
	{
	}

	D3DShader::~D3DShader()
	{
		OE_D3D_RELEASE(m_VertexShader);
		OE_D3D_RELEASE(m_PixelShader);
		OE_D3D_RELEASE(m_InputLayout);
	}

	void D3DShader::attachFromBinary(ShaderType type, const std::vector<char>& binary)
	{
		HRESULT hr;

		switch (type) {
		case ShaderType::VERTEX:
			hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateVertexShader(binary.data(), binary.size(), NULL, &m_VertexShader);
			m_InputLayout = GenerateInputLayoutFromByteCode(binary.data(), binary.size());
			break;
		case ShaderType::FRAGMENT:
			hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreatePixelShader(binary.data(), binary.size(), NULL, &m_PixelShader);
			break;
		}
		if (FAILED(hr))
			OE_LOG_FATAL("Error creating D3D shader!");
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
	}

	void D3DShader::finalize()
	{
		// Nothing
	}

	void D3DShader::bind() const
	{
		if (m_InputLayout)
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->IASetInputLayout(m_InputLayout);
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->VSSetShader(m_VertexShader ? m_VertexShader : 0, nullptr, 0);
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->PSSetShader(m_PixelShader ? m_PixelShader : 0, nullptr, 0);
	}

	void D3DShader::unbind() const
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->VSSetShader(NULL, nullptr, 0);
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->PSSetShader(NULL, nullptr, 0);
	}

	ID3D11InputLayout* D3DShader::GenerateInputLayoutFromByteCode(const void* byteCodeData, const unsigned long byteCodeLength) {
		ID3D11ShaderReflection* pReflector = NULL;

		HRESULT hr = D3DReflect(byteCodeData, byteCodeLength, IID_ID3D11ShaderReflection, (void**)&pReflector);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't relfect shader!");
			return nullptr;
		}

		D3D11_SHADER_DESC shaderDesc;
		pReflector->GetDesc(&shaderDesc);

		int byteOffset = 0;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (size_t i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pReflector->GetInputParameterDesc(i, &paramDesc);

			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = byteOffset;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
				byteOffset += 4;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
				byteOffset += 8;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				byteOffset += 12;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				byteOffset += 16;
			}

			inputLayoutDesc.push_back(elementDesc);
		}

		OE_D3D_RELEASE(pReflector);

		ID3D11InputLayout* inputLayout;
		hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), byteCodeData, byteCodeLength, &inputLayout);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't create a InputLayout from reflected shader!");
			return nullptr;
		}

		return inputLayout;
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