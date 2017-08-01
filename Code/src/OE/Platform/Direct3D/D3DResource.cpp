#include "OE/Platform/Direct3D/D3DResource.hpp"

namespace OrbitEngine { namespace Graphics {
	D3DResource::~D3DResource()
	{
		OE_D3D_RELEASE(m_pResource);
	}
} }
