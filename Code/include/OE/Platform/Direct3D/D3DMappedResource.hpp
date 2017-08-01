#ifndef GRAPHICS_D3DMAPPED_RESOURCE_HPP
#define GRAPHICS_D3DMAPPED_RESOURCE_HPP

#include "OE/Platform/Direct3D/D3DResource.hpp"
#include "OE/Graphics/API/MappedResource.hpp"

namespace OrbitEngine {	namespace Graphics {
	class D3DMappedResource : public D3DResource, public MappedResource {
	public:

		void* mapPointer(MappedPointerMode mode) override;
		void unmapPointer() override;
		void setData(const unsigned int count, const void* data) override;

		static D3D11_MAP PointerMapTypeToD3D(MappedPointerMode mode);
	protected:
		D3DMappedResource();

		D3D11_MAPPED_SUBRESOURCE m_MappedResource;
	};
} }

#endif