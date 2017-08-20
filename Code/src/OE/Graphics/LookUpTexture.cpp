#include "OE/Graphics/LookUpTexture.hpp"

namespace OrbitEngine { namespace Graphics {
	LookUpTexture::LookUpTexture()
		: p_Texture(0)
	{

	}

	LookUpTexture::~LookUpTexture()
	{
		if (p_Texture)
			delete p_Texture;
	}

	Texture* LookUpTexture::get()
	{
		return p_Texture;
	}

	void LookUpTexture::bind(unsigned int slot)
	{
		if (p_Texture)
			p_Texture->bind(slot);
	}
} }