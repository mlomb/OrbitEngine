#include "OE/Platform/Direct3D/D3DMappedResource.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine {	namespace Graphics {

	D3DMappedResource::D3DMappedResource()
	{
		ZeroMemory(&m_MappedResource, sizeof(m_MappedResource));

		m_MappedResource.DepthPitch = 0;
		m_MappedResource.RowPitch = 0;
	}

	void* D3DMappedResource::mapPointer(MappedPointerMode mode)
	{
		if (m_pResource == NULL)
			return NULL;
		// TODO See D3D10_MAP_FLAG_DO_NOT_WAIT
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->Map(m_pResource, 0, PointerMapTypeToD3D(mode), 0, &m_MappedResource);
		return m_MappedResource.pData;
	}

	void D3DMappedResource::unmapPointer()
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->Unmap(m_pResource, 0);
	}

	void D3DMappedResource::setData(unsigned int count, const void* data)
	{
		void* pointer = mapPointer(MappedPointerMode::WRITE);

		if (pointer == NULL)
			return;

		if (data == NULL)
			memset(pointer, 0, count);
		else
			memcpy(pointer, data, count);

		unmapPointer();
	}

	D3D11_MAP D3DMappedResource::PointerMapTypeToD3D(MappedPointerMode mode)
	{
		switch (mode)
		{
		case READ:
			return D3D11_MAP_READ;
		case WRITE:
			return D3D11_MAP_WRITE_DISCARD;
		}

		OE_ASSERT(false);
		return (D3D11_MAP)0;
	}
} }