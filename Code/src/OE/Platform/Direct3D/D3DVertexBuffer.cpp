#include "OE/Platform/Direct3D/D3DVertexBuffer.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	D3DVertexBuffer::D3DVertexBuffer(const unsigned int size, const void* data)
		: D3DBuffer(size, data, BufferType::VERTEX)
	{
	}

	void D3DVertexBuffer::bind() const {
		unsigned int stride = 0, offset = 0;

		if (p_Layout == nullptr)
			OE_LOG_WARNING("Trying to bind D3DVertexBuffer without setting the BufferLayout.")
		else
			stride = p_Layout->getStride();

		ID3D11Buffer* pBuffer = getD3DBuffer();
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->IASetVertexBuffers(0, 1, &pBuffer, &stride, &offset);
	}
} }