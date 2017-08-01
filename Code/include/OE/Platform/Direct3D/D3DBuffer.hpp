#ifndef GRAPHICS_D3DBUFFER_HPP
#define GRAPHICS_D3DBUFFER_HPP

#include "OE/Graphics/API/Buffer.hpp"
#include "OE/Platform/Direct3D/D3DMappedResource.hpp"

namespace OrbitEngine {	namespace Graphics {
	class D3DBuffer : public virtual Buffer, public D3DMappedResource {
	public:
		void resize(const unsigned int size) override;
		void setData(const unsigned int count, const void* data) override;

		inline ID3D11Buffer* getD3DBuffer() const { return static_cast<ID3D11Buffer*>(m_pResource); };
	public:
		D3D11_BUFFER_DESC m_BufferDescription;

		static D3D11_BIND_FLAG BufferTypeToD3D(BufferType type);
		static D3D11_USAGE BufferModeToD3D(BufferMode mode);
	protected:
		D3DBuffer(const unsigned int size, const void* data, BufferType type);
	};
} }

#endif