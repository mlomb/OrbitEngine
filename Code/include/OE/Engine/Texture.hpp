#ifndef ENGINE_TEXTURE_HPP
#define ENGINE_TEXTURE_HPP

#include "OE/Config.hpp"

#include "OE/Engine/Asset.hpp"
#include "OE/Meta/NativeReflection.hpp"
#include "OE/Graphics/API/Texture.hpp"

namespace OrbitEngine {	namespace Engine {

	class Texture /* : public Resource<Texture> */ {
		NATIVE_REFLECTION

	public:
		Texture(Graphics::Texture* tex);
		virtual ~Texture();

		void Assign(Graphics::Texture* tex);
		Graphics::Texture* GetNativeTexture();

	protected:
		Texture();

	private:
		Graphics::Texture* m_NativeTexture;
	};
} }

#endif
