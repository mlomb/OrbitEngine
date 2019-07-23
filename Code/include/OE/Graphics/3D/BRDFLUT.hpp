#ifndef GRAPHICS_BRDFLUT_HPP
#define GRAPHICS_BRDFLUT_HPP

#include "OE/Graphics/Textures/LookUpTexture.hpp"

namespace OrbitEngine { namespace Graphics {
	class FrameBuffer;

	// LUT to cache the second sum of the split sum approximation used by Unreal (see notes)
	// Bidirectional Reflectance Distribution Function
	class BRDFLUT : public LookUpTexture {
	public:
		BRDFLUT(unsigned short integratedBRDFSize = 128);
		~BRDFLUT();

	protected:
		FrameBuffer* m_BRDFFrameBuffer;
	};
} }

#endif