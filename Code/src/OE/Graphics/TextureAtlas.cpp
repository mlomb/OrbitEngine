#include "OE/Graphics/TextureAtlas.hpp"

#include "OE/Misc/JSON.hpp"
#include "OE/Graphics/API/Texture.hpp"
#include "OE/Graphics/2D/SpriteRenderer.hpp"

#undef GetObject

namespace OrbitEngine { namespace Graphics {

	TextureAtlas* TextureAtlas::Load(const std::string& metadata, const std::string& image)
	{
		rapidjson::Document meta = Misc::ReadJSON(metadata);
		if (!meta.IsObject())
			return nullptr;
		
		Texture* texture = Texture::Load(image);
		if (!texture)
			return nullptr;

		TextureAtlas* texture_atlas = new TextureAtlas(texture);
		texture_atlas->loadMetadata(meta.GetObject());
		return texture_atlas;
	}

	void TextureAtlas::drawFrame(FrameIndex index, const Math::Vec2f& position, const Math::Vec2f& size, SpriteRenderer& sr)
	{
		if (hasFrame(index)) {
			sr.bindTexture(m_Texture);
			sr.rect(position, size, getFrame(index).uvs);
		}
	}

	Math::Vec2f TextureAtlas::getTexelSize() const
	{
		const TextureProperties& props = m_Texture->getProperties();
		return Math::Vec2f(1.0f / (float)props.width, 1.0f / (float)props.height);
	}

	TextureAtlas::TextureAtlas(Texture* texture)
		: m_Texture(texture)
	{
	}

	TextureAtlas::~TextureAtlas()
	{
	}

} }