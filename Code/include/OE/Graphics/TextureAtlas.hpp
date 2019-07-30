#ifndef GRAPHICS_TEXTURE_ATLAS_HPP
#define GRAPHICS_TEXTURE_ATLAS_HPP

#include "Atlas.hpp"

namespace OrbitEngine { namespace Graphics {
	class SpriteRenderer;
	class Texture;

	/// A 2D texture atlas
	class TextureAtlas : public Atlas {
	public:
		~TextureAtlas();

		/**

		*/
		static TextureAtlas* Load(const std::string& metadata, const std::string& image);

		void drawFrame(FrameIndex index, const Math::Vec2f& position, const Math::Vec2f& size, SpriteRenderer& sr);
		Math::Vec2f getTexelSize() const override;
		Texture* getTexture() const;
		bool hasFrame(FrameIndex index) const;

	private:
		TextureAtlas(Texture* texture);

		Texture* m_Texture;
	};
} }

#endif