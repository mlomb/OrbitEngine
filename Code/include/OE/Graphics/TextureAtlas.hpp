#ifndef GRAPHICS_TEXTURE_ATLAS_HPP
#define GRAPHICS_TEXTURE_ATLAS_HPP

#include "Atlas.hpp"
#include "BitmapAtlas.hpp"

namespace OrbitEngine { namespace Graphics {
	class SpriteRenderer;
	class Texture;

	/**
		@brief A 2D texture atlas

		To generate a TextureAtlas check the BitmapAtlas class.
	*/
	class TextureAtlas : public Atlas {
	public:
		~TextureAtlas();

		Math::Vec2f getTexelSize() const override;

		/**
			@brief Draws a frame using a SpriteRenderer
			@param[in] index index of the frame to draw
			@param[in] position position to render the frame
			@param[in] size dimensions to render the frame
			@param[in] sr renderer to submit the draw
			@note If the provided index is not found the function will have no effect
		*/
		void drawFrame(FrameIndex index, const Math::Vec2f& position, const Math::Vec2f& size, SpriteRenderer& sr);

		/**
			@brief Creates a TextureAtlas from disk
			@param[in] metadata path to the metadata file to read (.json)
			@param[in] image path to the image file to read (.png)
			@return The TextureAtlas instance if the operation was successful, NULL otherwise
			@see Atlas for the atlas's metadata format
		*/
		static TextureAtlas* Load(const std::string& metadata, const std::string& image);

		template<typename T, unsigned int N>
		static TextureAtlas* CreateFromBitmap(const BitmapAtlas<T, N>& atlas);
	private:
		/**
			@brief Creates a texture atlas from a base texture.
			
			Only for internal use. Use \ref Load or \ref CreateFromBitmap to create a TextureAtlas.
		*/
		TextureAtlas(Texture* texture);

		Texture* m_Texture;
	};

	template<typename T, unsigned int N> TextureAtlas* Graphics::TextureAtlas::CreateFromBitmap(const BitmapAtlas<T, N>& atlas) {
		TextureAtlas* new_atlas;
		//new_atlas.m
		return new_atlas;
	}
} }

#endif