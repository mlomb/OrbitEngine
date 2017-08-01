#include "OE/Platform/Direct3D/D3DIndexBuffer.hpp"

#include "OE/Platform/Direct3D/D3DContext.hpp"

namespace OrbitEngine {	namespace Graphics {
	void D3DIndexBuffer::bind() const
	{
		Application::priv::D3DContext::GetCurrent()->getDeviceContext()->IASetIndexBuffer(getD3DBuffer(), DXGI_FORMAT_R16_UINT, 0);
	}
} }