#include "OE/Graphics/TextureBlitter.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {

	TextureBlitter::TextureBlitter()
		: m_SpriteBatcher(nullptr)
	{
	}

	TextureBlitter::~TextureBlitter()
	{
		m_PendingBlits.clear();
		if (m_SpriteBatcher)
			delete m_SpriteBatcher;
	}

	void TextureBlitter::blitNow(const BlitInfo& blit, FrameBuffer* dst)
	{
		FrameBuffer::Push(dst);

		m_PendingBlits.push_back(blit);
		doBlit(m_PendingBlits.size() - 1);
		m_PendingBlits.pop_back();

		FrameBuffer::Pop();
	}

	void TextureBlitter::commit(FrameBuffer* dst)
	{
		FrameBuffer::Push(dst);

		for (int i = 0; i < m_PendingBlits.size(); i += MAX_TEXTURES_PER_BLIT)
			doBlit(i);
		m_PendingBlits.clear();

		FrameBuffer::Pop();
	}

	void TextureBlitter::queueBlit(const BlitInfo& blit)
	{
		OE_ASSERT(blit.src);
		m_PendingBlits.push_back(blit);
	}

	void TextureBlitter::doBlit(int startIndex)
	{
		int stopIndex = std::min(startIndex + MAX_TEXTURES_PER_BLIT, (int)m_PendingBlits.size());

		Math::Vec2i size = FrameBuffer::GetCurrentSize();

		if (!m_SpriteBatcher) {
			m_SpriteBatcher = new SpriteBatcher();
		}

		m_SpriteBatcher->begin();
		m_SpriteBatcher->setPVMatrices(Math::Mat4::Orthographic(0, 1, 1, 0, 0, 1));

		for (int i = startIndex; i < stopIndex; i++) {
			const BlitInfo& blit = m_PendingBlits[i];

			m_SpriteBatcher->bindTexture(blit.src);
			m_SpriteBatcher->bindColor(blit.tint);
			m_SpriteBatcher->rect(
				Math::Vec2f(
					blit.dstPos.x / (float)size.x,
					blit.dstPos.y / (float)size.y
				),
				Math::Vec2f(
					blit.src->getProperties().width / (float)size.x,
					blit.src->getProperties().height / (float)size.y
				)
			);
		}

		m_SpriteBatcher->end();

		m_SpriteBatcher->bindTexture(nullptr);
	}
} }