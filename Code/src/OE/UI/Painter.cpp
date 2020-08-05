#include "OE/UI/Painter.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Graphics/API/VertexLayout.hpp"
#include "OE/Graphics/ShaderLoader.hpp"

#if OE_OPENGL_ANY
#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

namespace OrbitEngine { namespace UI {
	const float UI_EPS = 0.0001f;

	Painter::Painter()
		: m_pVertex(NULL), m_pIndex(NULL), m_VertexCount(0), m_IndexCount(0)
	{
		Graphics::VertexLayout* layout = new Graphics::VertexLayout();
		layout->push<Math::Vec2f>("POSITION");
		layout->push<Math::Color4f>("COLOR");
		layout->push<Math::Vec2f>("TEXCOORD"); // uvs
		layout->push<float>("TEXID"); // flags

		m_Mesh = Graphics::Mesh::Create(layout, true);
		m_Mesh->setTopology(Graphics::Topology::TRIANGLES);

		m_Shader = Graphics::ShaderLoader::UI();
		m_Shader->matchLayout(m_Mesh);

		m_Mesh->getVBO()->resize(4 * 10000 * sizeof(Vertex));
		m_Mesh->getIBO()->resize(6 * 10000 * sizeof(uint16_t));
		
		m_ConstantBuffer = Graphics::UniformsPack<ConstantBuffer>::Create();

		m_Atlas = new Graphics::DynamicAtlas();

#if OE_OPENGL_ANY
		if (Application::Context::GetCurrentAPI() == OPENGL
#if OE_OPENGL_ES
			|| Application::Context::GetCurrentAPI() == OPENGL_ES
#endif
			) {
			Graphics::GLShader* glShader = (Graphics::GLShader*)m_Shader;
			glShader->bind();
			glShader->setUniform1i("atlas", 0);
		}
#endif
	}

	Painter::~Painter()
	{
		OE_ASSERT(m_pVertex == NULL && m_pIndex == NULL);
		delete m_Mesh;
		delete m_Shader;
		delete m_Atlas;
	}

	void Painter::drawRectangle(const Math::Rectf& rect, const Math::Color4f& color)
	{
		if (rect.width < UI_EPS || rect.height < UI_EPS)
			return; // skip invalid rects
		if (color.a < UI_EPS)
			return; // skip transparent

		// 0     1
		// *-----*
		// | A  /|
		// |  /  |
		// |/  B |
		// *-----*
		// 2     3

		*m_pVertex = { rect.position,                               color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
		*m_pVertex = { rect.position + Math::Vec2f(rect.width, 0),  color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
		*m_pVertex = { rect.position + Math::Vec2f(0, rect.height), color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
		*m_pVertex = { rect.position + rect.size,                   color, Math::Vec2f(0, 0), 0 }; m_pVertex++;

		*m_pIndex = m_VertexCount + 0; m_pIndex++;
		*m_pIndex = m_VertexCount + 1; m_pIndex++;
		*m_pIndex = m_VertexCount + 2; m_pIndex++;
		*m_pIndex = m_VertexCount + 3; m_pIndex++;
		*m_pIndex = m_VertexCount + 2; m_pIndex++;
		*m_pIndex = m_VertexCount + 1; m_pIndex++;

		m_IndexCount += 6;
		m_VertexCount += 4;
	}

	void Painter::drawRoundedFan(const Math::Vec2f& position, const Math::Vec2f& radii, const Math::Color4f& color)
	{
		const int subdivisions = 6;
		const float step = 0.5f * PI / (float)subdivisions; // quarter

		//          A
		//        * |
		//     *    |
		//   *      | radii.y
		//  *       |
		// |--------X
		//   radii.x

		int cornerIndex = m_VertexCount;
		Math::Vec2f center = position + radii; // X

		*m_pVertex = { center, color, Math::Vec2f(0, 0), 0 }; m_pVertex++; // X
		*m_pVertex = { center - Math::Vec2f(0, radii.y), color, Math::Vec2f(0, 0), 0 }; m_pVertex++; // A
		m_VertexCount += 2;

		for (int i = 0; i < subdivisions; i++) {
			float angle = 0.5f * PI + (i + 1) * step;
			Math::Vec2f offset = radii * Math::Vec2f(cos(angle), -sin(angle));

			*m_pVertex = { center + offset, color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
			m_VertexCount++;

			*m_pIndex = cornerIndex; m_pIndex++;
			*m_pIndex = m_VertexCount - 1; m_pIndex++;
			*m_pIndex = m_VertexCount - 2; m_pIndex++;
			m_IndexCount += 3;
		}
	}

	void Painter::drawRoundedCorner(const Math::Rectf& rect, const Math::Vec2f& radii, const Math::Color4f& color)
	{
		if (radii.x < UI_EPS && radii.y < UI_EPS) {
			// No radius, just fill with a normal rectangle
			// -------
			// |     |
			// |     |
			// -------
			drawRectangle(rect, color);
			return;
		}

		// Draw the corner with radius and fill the remaining space with normal rectangles
		//    __------
		//   *  |    |
		//  *\  |    |
		// * A\ |    |
		// |___\|  B |
		// |    |    |
		// | C  |    |
		// |    |    |
		// -----------

		// A
		drawRoundedFan(rect.position, radii, color);

		if (rect.width > radii.x) {
			// B
			drawRectangle(Math::Rectf(rect.x + radii.x, rect.y, rect.width - radii.x, rect.height), color);
		}
		if (rect.height > radii.y) {
			// C
			drawRectangle(Math::Rectf(rect.x, rect.y + radii.y, rect.width, rect.height - radii.y), color);
		}
	}

	void Painter::drawRoundedRectangle(const Math::Vec2f& position, const Math::Vec2f& size, const Math::Color4f& color, const RoundedRectParams& params)
	{
	}

	void Painter::drawText(const Graphics::TextLayout& textLayout, const Math::Vec2f& position)
	{
		using namespace OrbitEngine::Graphics;

		if (textLayout.glyphs.size() == 0 || !textLayout.font)
			return;

		// the texture is POT
		const float texelSize = 1.0f / (float)m_Atlas->getSize();

		BitmapRGBA bitmap;
		const Math::Vec2f base_pos(position.x, position.y);
		Math::Recti rect;

		for (const TextLayout::GlyphInstance& gi : textLayout.glyphs) {
			DynamicAtlas::Index idx = textLayout.settings.size + gi.codepoint * 1000; // TODO: include font as hash

			if (!m_Atlas->tryGet(idx, rect)) {
				// new glyph
				GlyphMetrics metrics;
				if (!textLayout.font->getGlyph(gi.codepoint, textLayout.settings.size, GlyphRenderMode::GRAY, bitmap, metrics)) {
					OE_LOG_DEBUG("Can't get Glyph!");
					continue;
				}

				if (bitmap.valid()) {
					Texture* glyph_tex = bitmap.toTexture();
					if (!m_Atlas->add(idx, rect, glyph_tex)) {
						OE_ASSERT_MSG(false, "Can't add Glyph to atlas!");
						continue;
					}
					m_DestroyAfterCommitingAtlas.push_back(glyph_tex);
				}
				else {
					OE_ASSERT(false);
				}
			}

			const Math::Vec2f pos = base_pos + Math::Vec2f(gi.rect.x, gi.rect.y);
			const Math::Vec2f size = Math::Vec2f(gi.rect.width, gi.rect.height);
			const Math::Color4f color = Math::Color::White;

			float u0 = texelSize * rect.x;
			float v0 = texelSize * rect.y;
			float u1 = texelSize * (rect.x + rect.width);
			float v1 = texelSize * (rect.y + rect.height);

			v0 = 1.0f - v0;
			v1 = 1.0f - v1;

			*m_pVertex = { pos,                          color, Math::Vec2f(u0, v1), 1 }; m_pVertex++;
			*m_pVertex = { pos + Math::Vec2f(0, size.y), color, Math::Vec2f(u0, v0), 1 }; m_pVertex++;
			*m_pVertex = { pos + size,                   color, Math::Vec2f(u1, v0), 1 }; m_pVertex++;
			*m_pVertex = { pos + Math::Vec2f(size.x, 0), color, Math::Vec2f(u1, v1), 1 }; m_pVertex++;

			*m_pIndex = m_VertexCount + 2; m_pIndex++;
			*m_pIndex = m_VertexCount + 1; m_pIndex++;
			*m_pIndex = m_VertexCount + 0; m_pIndex++;
			*m_pIndex = m_VertexCount + 0; m_pIndex++;
			*m_pIndex = m_VertexCount + 3; m_pIndex++;
			*m_pIndex = m_VertexCount + 2; m_pIndex++;

			m_IndexCount += 6;
			m_VertexCount += 4;
		}
	}

	void Painter::setProjection(const Math::Mat4& proj)
	{
		m_ConstantBufferData.pr_matrix = proj;
	}

	void Painter::begin()
	{
		m_pVertex = (Vertex*)m_Mesh->getVBO()->mapPointer(Graphics::MappedPointerMode::WRITE);
		m_pIndex = (uint16_t*)m_Mesh->getIBO()->mapPointer(Graphics::MappedPointerMode::WRITE);

		m_VertexCount = 0;
		m_IndexCount = 0;
	}

	void Painter::flush()
	{
		end();
		begin();
	}

	void Painter::end()
	{
		m_Atlas->commit();

		// destroy temporal textures
		for (Graphics::Texture* tex : m_DestroyAfterCommitingAtlas)
			delete tex;
		m_DestroyAfterCommitingAtlas.clear();

		/*
		auto pos = Math::Vec2f(0, 0);
		auto size = Math::Vec2f(m_Atlas->getSize(), m_Atlas->getSize());
		*m_pVertex = { pos,                          Math::Color::White, Math::Vec2f(0, 0), 2 }; m_pVertex++;
		*m_pVertex = { pos + Math::Vec2f(0, size.y), Math::Color::White, Math::Vec2f(0, 1), 2 }; m_pVertex++;
		*m_pVertex = { pos + size,                   Math::Color::White, Math::Vec2f(1, 1), 2 }; m_pVertex++;
		*m_pVertex = { pos + Math::Vec2f(size.x, 0), Math::Color::White, Math::Vec2f(1, 0), 2 }; m_pVertex++;
		*m_pIndex = m_VertexCount + 2; m_pIndex++;
		*m_pIndex = m_VertexCount + 1; m_pIndex++;
		*m_pIndex = m_VertexCount + 0; m_pIndex++;
		*m_pIndex = m_VertexCount + 0; m_pIndex++;
		*m_pIndex = m_VertexCount + 3; m_pIndex++;
		*m_pIndex = m_VertexCount + 2; m_pIndex++;
		m_IndexCount += 6;
		m_VertexCount += 4;
		*/
		//drawRoundedRectTest(Math::Vec2f(200, 200), Math::Vec2f(50, 200), 0, 0, Math::Color::Red, Math::Color::Green);
		//drawBorderedFan(Math::Vec2f(200, 200), Math::Vec2f(5,5), 1, 1, Math::Color::Red);
		/*
		RoundedRectParams params;
		params.cornerRadii[0] = Math::Vec2f(15, 15);
		params.cornerRadii[1] = Math::Vec2f(15, 15);
		params.cornerRadii[2] = Math::Vec2f(15, 15);
		params.cornerRadii[3] = Math::Vec2f(15, 15);
		drawRoundedRectangle(Math::Vec2f(200, 200), Math::Vec2f(50, 50), Math::Color::Red, params);
		*/
		drawRoundedCorner(Math::Rectf(200, 200, 500, 500), Math::Vec2f(0, 0), Math::Color::Magenta);
		drawRoundedCorner(Math::Rectf(200, 200, 500, 500), Math::Vec2f(200, 200), Math::Color::Red);

		m_Mesh->getVBO()->unmapPointer();
		m_Mesh->getIBO()->unmapPointer();
		m_pVertex = 0;
		m_pIndex = 0;
		
		Graphics::States* states = Application::priv::ContextImpl::GetCurrent()->getGlobalStates();
		states->setBlending(Graphics::BlendState::SRC_ALPHA);
		states->setDepthTest(Graphics::FunctionMode::DISABLED);
		states->setStencil(Graphics::FunctionMode::DISABLED);
		states->setCullMode(Graphics::CullMode::NONE);

		m_Shader->bind();
		m_ConstantBuffer->bind("PVMatrices", m_Shader);
		m_ConstantBuffer->setData(m_ConstantBufferData);

		m_Atlas->getTexture()->bind(0);

		m_Mesh->drawIndexed(m_IndexCount);
	}
} }
