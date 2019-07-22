#ifndef GRAPHICS_SPRITE_BATCHER_HPP
#define GRAPHICS_SPRITE_BATCHER_HPP

#include "OE/Graphics/2D/SpriteRenderer.hpp"
#include "OE/Graphics/2D/BatchRenderer2D.hpp"
#include "OE/Graphics/2D/TextureArrayHandler.hpp"

#include "OE/Math/Color.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {
	
	struct Batch2DVertex {
		Math::Vec2f position;
		Math::Color color;
		Math::Vec2f uv;
		float tid;
	};

	/*
		Supports textures and colors
	*/
	class SpriteBatcher : public SpriteRenderer, public BatchRenderer2D<Batch2DVertex>, virtual public TextureArrayHandler {
	public:
		SpriteBatcher(int numSprites = 50000);
		~SpriteBatcher();

		void begin() override;
		void end() override;

	private:
		// TODO: should handleTexture work like this?
		int handleTexture(Texture* tex) override;

		void submitRect(const Math::Vec2f(&positions)[4], const Math::Vec2f(&uvs)[4], const Math::Color& color, unsigned int texture) override;
	};
} }

#endif