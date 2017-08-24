#include "OE/Platform/Direct3D/D3DBuffer.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Graphics {
	D3DBuffer::D3DBuffer(const unsigned int size, const void* data, BufferType type)
	{
		ZeroMemory(&m_BufferDescription, sizeof(m_BufferDescription));

		m_BufferDescription.Usage = BufferModeToD3D(BufferMode::DYNAMIC);
		m_BufferDescription.BindFlags = BufferTypeToD3D(type);
		m_BufferDescription.CPUAccessFlags = m_BufferDescription.Usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;
		m_BufferDescription.MiscFlags = 0;
		m_BufferDescription.StructureByteStride = 0;

		if (type == BufferType::CONSTANT) {
			if (size % 16 != 0) {
				void* newData = 0;
				int newSize = (size + 15) & -16;
				if (data != 0) {
					newData = malloc(newSize);
					memcpy(newData, data, size);
				}
				setData(newSize, newData);
				//OE_LOG_WARNING("Constant buffers must size in multiples of 16 bytes.");
				return;
			}
		}

		setData(size, data);
	}

	void D3DBuffer::setData(const unsigned int count, const void* data) {
		if (m_BufferDescription.ByteWidth < count)
			resize(count);

		D3DMappedResource::setData(count, data);
	}

	void D3DBuffer::resize(const unsigned int size)
	{
		m_BufferDescription.ByteWidth = size;
		p_Size = size;

		ID3D11Buffer* pBuffer;
		Application::priv::D3DContext::GetCurrent()->getDevice()->CreateBuffer(&m_BufferDescription, 0, &pBuffer);
		m_pResource = pBuffer;
	}

	D3D11_BIND_FLAG D3DBuffer::BufferTypeToD3D(BufferType type)
	{
		switch (type) {
		case BufferType::VERTEX:
			return D3D11_BIND_VERTEX_BUFFER;
		case BufferType::INDEX:
			return D3D11_BIND_INDEX_BUFFER;
		case BufferType::CONSTANT:
			return D3D11_BIND_CONSTANT_BUFFER;
		}

		OE_ASSERT(false);
		return (D3D11_BIND_FLAG)0;
	}

	D3D11_USAGE D3DBuffer::BufferModeToD3D(BufferMode mode)
	{
		switch (mode) {
		case BufferMode::DEFAULT:
			return D3D11_USAGE_DEFAULT;
		case BufferMode::DYNAMIC:
			return D3D11_USAGE_DYNAMIC;
		}

		return D3D11_USAGE_DEFAULT;
	}
} }