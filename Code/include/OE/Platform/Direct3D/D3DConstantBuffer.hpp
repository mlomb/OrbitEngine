#ifndef GRAPHICS_D3DCONSTANT_BUFFER_HPP
#define GRAPHICS_D3DCONSTANT_BUFFER_HPP

#include "OE/Graphics/API/UniformsPack.hpp"

#include "OE/Platform/Direct3D/D3DBuffer.hpp"
#include "OE/Platform/Direct3D/D3DContext.hpp"

namespace OrbitEngine { namespace Graphics {
	template <class CB>
	class D3DConstantBuffer : public UniformsPack<CB>, public D3DBuffer {
	public:
		D3DConstantBuffer() : D3DBuffer(sizeof(CB), 0, BufferType::CONSTANT) {};

		void bind(unsigned int slot, ShaderType shader) const override;
		void bind() const override;

		void setData(const CB& data) override;
	};

	template<class CB>
	inline void D3DConstantBuffer<CB>::setData(const CB& data)
	{
		D3DMappedResource::setData(sizeof(CB), &data);
	}

	template<class CB>
	inline void D3DConstantBuffer<CB>::bind(unsigned int slot, ShaderType shader) const
	{
		ID3D11Buffer* pBuffer = getD3DBuffer();

		switch (shader) {
		case ShaderType::VERTEX:
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->VSSetConstantBuffers(slot, 1, &pBuffer);
			return;
		case ShaderType::FRAGMENT:
			Application::priv::D3DContext::GetCurrent()->getDeviceContext()->PSSetConstantBuffers(slot, 1, &pBuffer);
			return;
		}

		OE_ASSERT(false);
	}

	template<class CB>
	inline void D3DConstantBuffer<CB>::bind() const
	{
		bind(0, ShaderType::VERTEX);
		bind(0, ShaderType::FRAGMENT);
	}
} }

#endif