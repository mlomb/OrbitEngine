#ifndef GRAPHICS_TEXTURE_ARRAY_HANDLER_HPP
#define GRAPHICS_TEXTURE_ARRAY_HANDLER_HPP

#include <vector>

#define OE_2D_MAX_TEXTURES 16

namespace OrbitEngine { namespace Graphics {

	class Texture;
	class Shader;

	class TextureArrayHandler {
	public:
		int handleTexture(Texture* tex);

	protected:
		void init(Shader* shader);
		void begin();
		void end();

		std::vector<Texture*> p_Textures;
	};

} }

#endif