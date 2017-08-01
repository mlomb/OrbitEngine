#ifndef GRAPHICS_D3DINDEX_BUFFER_HPP
#define GRAPHICS_D3DINDEX_BUFFER_HPP

#include "OE/Platform/Direct3D/D3DBuffer.hpp"
#include "OE/Platform/Direct3D/D3DMappedResource.hpp"

namespace OrbitEngine {	namespace Graphics {
	class D3DIndexBuffer : public D3DBuffer {
	public:
		D3DIndexBuffer(const unsigned int size, const void* data)
			: D3DBuffer(size, data, BufferType::INDEX) {};

		void bind() const override;

	public:
		// What
		void* mapPointer(MappedPointerMode mode) override { return D3DMappedResource::mapPointer(mode); };
		void unmapPointer() override { return D3DMappedResource::unmapPointer(); };
	};
} }

#endif