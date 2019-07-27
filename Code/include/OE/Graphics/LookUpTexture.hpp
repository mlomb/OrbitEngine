#ifndef GRAPHICS_LOOKUP_TEXTURE_HPP
#define GRAPHICS_LOOKUP_TEXTURE_HPP

#include "OE/Graphics/API/Texture.hpp"

namespace OrbitEngine { namespace Graphics {
	class LookUpTexture {
	public:
		virtual ~LookUpTexture();

		Texture* get();
		void bind(unsigned int slot);

	protected:
		LookUpTexture();

		Texture* p_Texture;
	};
} }

#endif