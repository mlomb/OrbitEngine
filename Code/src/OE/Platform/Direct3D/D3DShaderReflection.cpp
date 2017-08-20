#include "OE/Platform/Direct3D/D3DShaderReflection.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Graphics {
	D3DShaderReflection::D3DShaderReflection()
	{

	}

	D3DShaderReflection::~D3DShaderReflection()
	{

	}

	void D3DShaderReflection::reflect(const std::vector<char>& binary, ShaderType type)
	{
		ID3D11ShaderReflection* pReflector = NULL;

		HRESULT hr = D3DReflect(binary.data(), binary.size(), IID_ID3D11ShaderReflection, (void**)&pReflector);
		if (FAILED(hr)) {
			OE_LOG_FATAL("Can't relfect shader!");
			return;
		}

		D3D11_SHADER_DESC shaderDesc;
		pReflector->GetDesc(&shaderDesc);

		reflectBuffers(shaderDesc, pReflector, type);

		if (type == ShaderType::VERTEX) {
			// We need the input layout only from the Vertex Shader
			auto inputLayoutDesc = reflectInputLayout(shaderDesc, pReflector);
			hr = Application::priv::D3DContext::GetCurrent()->getDevice()->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), binary.data(), binary.size(), &m_VertexInputLayout);
			if (FAILED(hr))
				OE_LOG_WARNING("Can't create a InputLayout from reflected shader!");
		}

		OE_D3D_RELEASE(pReflector);
	}
	
	std::vector<D3D11_INPUT_ELEMENT_DESC> D3DShaderReflection::reflectInputLayout(D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* reflector)
	{
		HRESULT hr;

		int byteOffset = 0;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (size_t i = 0; i < shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			reflector->GetInputParameterDesc(i, &paramDesc);

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

		return inputLayoutDesc;
	}

	void D3DShaderReflection::reflectBuffers(D3D11_SHADER_DESC& shaderDesc, ID3D11ShaderReflection* reflector, ShaderType type)
	{
		for (unsigned int i = 0; i < shaderDesc.ConstantBuffers; ++i)
		{
			unsigned int register_index = 0;
			ID3D11ShaderReflectionConstantBuffer* buffer = NULL;
			buffer = reflector->GetConstantBufferByIndex(i);

			D3D11_SHADER_BUFFER_DESC bdesc;
			buffer->GetDesc(&bdesc);

			for (unsigned int k = 0; k < shaderDesc.BoundResources; ++k)
			{
				D3D11_SHADER_INPUT_BIND_DESC ibdesc;
				reflector->GetResourceBindingDesc(k, &ibdesc);

				if (!strcmp(ibdesc.Name, bdesc.Name))
					register_index = ibdesc.BindPoint;
			}

			ShaderBufferReflection shaderbuffer;
			shaderbuffer.name = bdesc.Name;
			shaderbuffer.slot = register_index;
			shaderbuffer.shaderType = type;
			addBuffer(shaderbuffer);
		}
	}

	ID3D11InputLayout* D3DShaderReflection::getVertexInputLayout()
	{
		return  m_VertexInputLayout;
	}
} }