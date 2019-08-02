#include "OE/Platform/Direct3D/D3DVertexBuffer.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	D3DVertexBuffer::D3DVertexBuffer(const unsigned int size, const void* data)
		: D3DBuffer(size, data, BufferType::VERTEX), m_InputLayout(NULL)
	{
	}

	D3DVertexBuffer::~D3DVertexBuffer()
	{
		if(m_InputLayout)
			OE_D3D_RELEASE(m_InputLayout);
	}

	void D3DVertexBuffer::buildInputLayout() {
		if (p_Layout) {
			////////////////////////////////////////
			// Hacky way to create InputLayouts   //
			// without keeping a reference to the //
			// original shader blob      :)       //
			////////////////////////////////////////
			std::string vertexShader = "struct VS_INPUT {";

			std::vector<D3D11_INPUT_ELEMENT_DESC> input_elements;
			for (const VertexElement& source_elem : p_Layout->getElements()) {
				D3D11_INPUT_ELEMENT_DESC elem;
				elem.SemanticIndex = 0;
				elem.SemanticName = source_elem.name.c_str();
				elem.Format = static_cast<DXGI_FORMAT>(source_elem.type);
				elem.InputSlot = 0;
				elem.AlignedByteOffset = source_elem.offset;
				elem.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				elem.InstanceDataStepRate = 0;
				input_elements.push_back(elem);

				std::string type = "";

				switch (elem.Format) {
				case DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT:
					type = "float";
					break;
				case DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT:
					type = "float2";
					break;
				case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
				case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
					type = "float4";
					break;
				default:
					OE_LOG_WARNING("DXGI_FORMAT not supported: " << elem.Format);
					break;
				}

				vertexShader += type + " input_" + source_elem.name + " : " + source_elem.name + ";";
			}

			vertexShader += "}; struct PS_INPUT {}; PS_INPUT main(VS_INPUT input) { PS_INPUT output; return output; }";

			ID3D10Blob* shaderBlob = NULL;
			ID3D11VertexShader* vertexShaderPtr = NULL;

			D3DCompile(vertexShader.c_str(), vertexShader.size(), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &shaderBlob, NULL);
			if (shaderBlob == NULL) {
				OE_LOG_WARNING("Error compiling shader to generate the input layout!");
				return;
			}
			if (Application::priv::D3DContext::GetCurrent()->getDevice()->CreateVertexShader((DWORD*)shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShaderPtr) != S_OK) {
				OE_LOG_WARNING("Error creating shader to generate the input layout!");
				OE_D3D_RELEASE(shaderBlob);
				return;
			}
			if (Application::priv::D3DContext::GetCurrent()->getDevice()->CreateInputLayout(&input_elements[0], input_elements.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &m_InputLayout) != S_OK) {
				OE_LOG_WARNING("Error creating input layout!");
			}

			OE_D3D_RELEASE(vertexShaderPtr);
			OE_D3D_RELEASE(shaderBlob);
		}
		else
			OE_LOG_WARNING("Cant create input layout without a VertexLayout!");
	}

	void D3DVertexBuffer::bind() const {
		if (!p_Layout)
			return;

		unsigned int stride = 0, offset = 0;

		stride = p_Layout->getStride();

		if (m_InputLayout)
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->IASetInputLayout(m_InputLayout);

		ID3D11Buffer* pBuffer = getD3DBuffer();
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
	}
} }