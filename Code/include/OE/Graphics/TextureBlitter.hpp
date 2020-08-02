#ifndef GRAPHICS_TEXTURE_BLITTER_HPP
#define GRAPHICS_TEXTURE_BLITTER_HPP

#include <vector>

#include "OE/Math/Vec4.hpp"
#include "OE/Graphics/API/Texture.hpp"
#include "OE/Graphics/API/FrameBuffer.hpp"
#include "OE/Graphics/2D/SpriteBatcher.hpp"

namespace OrbitEngine { namespace Graphics {

	const int MAX_TEXTURES_PER_BLIT = 8;

	struct BlitInfo {
		Texture* src;
		Math::Vec2i dstPos;
		Math::Color4f tint = Math::Color::White;
	};

	class TextureBlitter {
	public:
		TextureBlitter();
		~TextureBlitter();

		void commit(FrameBuffer* dst);
		void queueBlit(const BlitInfo& blit);

		void blitNow(const BlitInfo& blit, FrameBuffer* dst);

	private:
		std::vector<BlitInfo> m_PendingBlits;
		SpriteBatcher* m_SpriteBatcher;

		void doBlit(int startIndex);
	};

} }

#endif