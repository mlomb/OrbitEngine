#ifndef GRAPHICS_D3DVERTEX_BUFFER_HPP
#define GRAPHICS_D3DVERTEX_BUFFER_HPP

#include "OE/Platform/Direct3D/D3DBuffer.hpp"

namespace OrbitEngine {	namespace Graphics {
	class VertexLayout;

	class D3DVertexBuffer : public D3DBuffer, public VertexBuffer {
	public:
		D3DVertexBuffer(const unsigned int size, const void* data);

		void bind() const override;

	private:

	public:
		// What
		void* mapPointer(MappedPointerMode mode) override { return D3DMappedResource::mapPointer(mode); };
		void unmapPointer() override { return D3DMappedResource::unmapPointer(); };
	};
} }

#endif