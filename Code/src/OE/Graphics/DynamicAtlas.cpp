#include "OE/Graphics/DynamicAtlas.hpp"

#include "OE/Graphics/DynamicAtlasAllocator.hpp"
#include "OE/Graphics/TextureBlitter.hpp"

namespace OrbitEngine { namespace Graphics {
	DynamicAtlas::DynamicAtlas()
		: m_AtlasRT(nullptr)
	{
		m_Allocator = new DynamicAtlasAllocator(64, 4096);
		m_TextureBlitter = new TextureBlitter();
	}

	DynamicAtlas::~DynamicAtlas()
	{
		m_Entries.clear();
		delete m_Allocator;
		delete m_TextureBlitter;
		if (m_AtlasRT)
			delete m_AtlasRT;
	}

	bool DynamicAtlas::tryGet(Index index, Math::Recti& rect) const
	{
		auto it = m_Entries.find(index);
		if (it != m_Entries.end()) {
			rect = (*it).second;
			return true;
		}
		return false;
	}

	bool DynamicAtlas::add(Index index, Math::Recti& rect, Texture* texture)
	{
		// TODO: we should receive a strong texture ptr until blitted

		if (!texture)
			return false;
		if (tryGet(index, rect))
			return true;

		auto props = texture->getProperties();

		rect.width = props.width;
		rect.height = props.height;

		// allocate
		unsigned int lx, ly;
		if (m_Allocator->tryAllocate(props.width, props.height, lx, ly)) {
			rect.x = lx;
			rect.y = ly;

			m_Entries.insert(std::make_pair(index, rect));

			BlitInfo bi;
			bi.src = texture;
			bi.dstPos = Math::Vec2i(lx, ly);
			m_TextureBlitter->queueBlit(bi);

			return true;
		}

		return false;
	}

	void DynamicAtlas::commit()
	{
		if (!m_AtlasRT || m_AtlasRT->getWidth() != m_Allocator->getSize()) {
			FrameBuffer* new_atlas = FrameBuffer::Create(m_Allocator->getSize(), m_Allocator->getSize());
			Graphics::TextureFormatProperties props;
			props.format = Graphics::RGBA;
			props.dataType = Graphics::UNSIGNED_BYTE;
			new_atlas->attachColorTextures(1, props);
			new_atlas->finalize();
			new_atlas->clear();
			if (m_AtlasRT) {
				// blit old atlas into the new one
				BlitInfo bi;
				bi.dstPos = Math::Vec2i(0, 0);
				bi.src = getTexture();
				m_TextureBlitter->blitNow(bi, new_atlas);
				delete m_AtlasRT;
			}
			m_AtlasRT = new_atlas;
		}
		m_TextureBlitter->commit(m_AtlasRT);
	}

	Texture* DynamicAtlas::getTexture() const
	{
		return m_AtlasRT ? m_AtlasRT->getColorTextures()[0] : nullptr;
	}
} }