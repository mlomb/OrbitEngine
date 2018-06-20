#include "OE/Engine/Texture.hpp"

namespace OrbitEngine {	namespace Engine {

	Texture::Texture()
	{
	}
	
	Texture::Texture(Graphics::Texture* tex)
		: m_NativeTexture(tex)
	{
	}

	Texture::~Texture()
	{
		delete m_NativeTexture;
	}

	void Texture::Assign(Graphics::Texture* tex)
	{
		if (m_NativeTexture)
			delete m_NativeTexture;
		m_NativeTexture = tex;
	}

	Graphics::Texture* Texture::GetNativeTexture()
	{
		return m_NativeTexture;
	}

	NATIVE_REFLECTION_BEGIN(Texture)
	NATIVE_REFLECTION_END()
} }