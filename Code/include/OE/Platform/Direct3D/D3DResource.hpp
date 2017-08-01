#ifndef GRAPHICS_D3DRESOURCE_HPP
#define GRAPHICS_D3DRESOURCE_HPP

#include "OE/Platform/Direct3D/Direct3D.hpp"

namespace OrbitEngine {	namespace Graphics {
	class D3DResource {
	public:
	protected:
		~D3DResource();

		ID3D11Resource* m_pResource;
	};
} }

#endif