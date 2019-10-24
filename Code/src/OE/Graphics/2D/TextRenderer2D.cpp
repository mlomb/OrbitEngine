#include "OE/Graphics/2D/TextRenderer2D.hpp"

#include "OE/Graphics/ShaderLoader.hpp"
#include "OE/Graphics/TextureAtlas.hpp"
#include "OE/Graphics/API/States.hpp"

namespace OrbitEngine { namespace Graphics {

	TextRenderer2D::TextRenderer2D(unsigned int batchSize)
		: BatchRenderer2D<TextRenderer2DVertex>(
			([]() {
				VertexLayout* layout = new VertexLayout();
				layout->push<Math::Vec2f>("POSITION"); // Vertex position
				layout->push<unsigned char>("COLOR1", 4, true);  // Text color
				layout->push<unsigned char>("COLOR2", 4, true);  // Border Color
				layout->push<Math::Vec2f>("TEXCOORD"); // UV's
				layout->push<float>("TEXID");		   // TextureID
				layout->push<float>("TEXID2");		   // Glyph render mode
				layout->push<float>("TEXID3");		   // Border factor
				return layout;
			})(),
			batchSize * 4,
			Topology::TRIANGLES
			)
	{
		m_Shader = ShaderLoader::Text2D();
		TextureArrayHandler::init(m_Shader);
	}

	TextRenderer2D::~TextRenderer2D()
	{
		delete m_Shader;
	}

	void TextRenderer2D::begin()
	{
		BatchRenderer2D::begin();
		TextureArrayHandler::begin();
		m_IndexCount = 0;
	}

	void TextRenderer2D::end()
	{
		m_Shader->bind();
		m_UPVMatrix->bind("PVMatrix", m_Shader);

		BatchRenderer2D::end();
		TextureArrayHandler::end();

		States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();

		states->setBlending(BlendState::SRC_ALPHA);
		states->setDepthTest(FunctionMode::DISABLED);
		states->setStencil(FunctionMode::DISABLED);
		states->setCullMode(CullMode::NONE);

		m_Mesh->drawIndexed(m_IndexCount);
	}

	void TextRenderer2D::drawText(const std::vector<GlyphCodepoint>& text, const Math::Vec2f& position, TextStyle style, FontCollection* font) {
		if (text.size() == 0)
			return;

		Math::Vec2f pen(0, 0);
		pen.x += position.x;
		pen.y += position.y;

		int texture = handleTexture(font->m_TextureAtlas->getTexture());

		for (unsigned int i = 0; i < text.size(); i++) {
			GlyphCodepoint code = text[i];
			if (code == '\n' || code == '\r') {
				pen.x = position.x;
				pen.y += style.size;
				continue;
			}

			const auto& it = font->m_Collection.find(code);
			if (it == font->m_Collection.end())
				continue;

			const auto& p = *(*it).second.rbegin(); // TODO: use a proper sort: mode desc, size desc
			GlyphRenderMode mode = p.first;
			bool sdf = mode != GlyphRenderMode::GRAY && mode != GlyphRenderMode::COLOR; // some kind of SDF
			FrameIndex index = FontCollection::toIndex(code, mode);
			const FontCollection::Entry& entry = p.second;
			
			// resized metrics
			float scale = style.size / (float)entry.original_size;
			float off = sdf ? 5.0f * scale : 0;
			
			float width = (float)entry.metrics.width * scale + 2.0f * off;
			float height = (float)entry.metrics.height * scale + 2.0f * off;
			float bearingX = (float)entry.metrics.H_bearingX * scale;
			float bearingY = (float)entry.metrics.H_bearingY * scale;
			float advance = (float)entry.metrics.H_advance * scale;
			
			if (font->m_TextureAtlas->hasFrame(index) && entry.metrics.width > 0 && entry.metrics.height > 0) {
				Math::Vec2f pos = pen + Math::Vec2f(bearingX, -bearingY);
				const TextureAtlas::Frame& frame = font->m_TextureAtlas->getFrame(index);
				
				if (sdf) {
					pos.x -= off;
					pos.y -= off;
				}
				
				pushVertex({ p_Transform * (pos + Math::Vec2f(0, 0)),          style.color, style.borderColor, frame.uvs._1, (float)texture, (float)mode, style.border });
				pushVertex({ p_Transform * (pos + Math::Vec2f(0, height)),     style.color, style.borderColor, frame.uvs._2, (float)texture, (float)mode, style.border });
				pushVertex({ p_Transform * (pos + Math::Vec2f(width, height)), style.color, style.borderColor, frame.uvs._3, (float)texture, (float)mode, style.border });
				pushVertex({ p_Transform * (pos + Math::Vec2f(width, 0)),      style.color, style.borderColor, frame.uvs._4, (float)texture, (float)mode, style.border });

				m_IndexCount += 6;
			}

			if (i + 1 < text.size()) {
				const auto& kit = entry.kernings.find((GlyphCodepoint)text[i + 1]);
				if(kit != entry.kernings.end())
					pen.x += (float)(*kit).second * scale;
			}
			
			pen.x += advance;
		}
	}

	Math::Vec2f TextRenderer2D::getBounds(const std::vector<GlyphCodepoint>& text, TextStyle style, FontCollection* font)
	{
		if (text.size() == 0)
			return Math::Vec2i(0, 0);

		Math::Vec2f pen(0, 0);
		float maxWidth = 0.0f;
		int lines = 1;

		for (unsigned int i = 0; i < text.size(); i++) {
			GlyphCodepoint code = text[i];
			if (code == '\n' || code == '\r') {
				pen.x = 0;
				lines++;
				continue;
			}

			const auto& it = font->m_Collection.find(code);
			if (it == font->m_Collection.end())
				continue;

			const auto& p = *(*it).second.rbegin();
			const FontCollection::Entry& entry = p.second;

			float scale = style.size / (float)entry.original_size;

			if (i + 1 < text.size()) {
				const auto& kit = entry.kernings.find((GlyphCodepoint)text[i + 1]);
				if (kit != entry.kernings.end())
					pen.x += (float)(*kit).second * scale;
			}

			pen.x += (float)entry.metrics.H_advance * scale;

			maxWidth = std::max(maxWidth, pen.x);
		}

		return Math::Vec2f(maxWidth, lines * style.size);
	}

} }
