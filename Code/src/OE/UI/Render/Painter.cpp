#include "OE/UI/Render/Painter.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Graphics/API/VertexLayout.hpp"
#include "OE/Graphics/API/States.hpp"
#include "OE/Graphics/ShaderLoader.hpp"

#if OE_OPENGL_ANY
#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

namespace OrbitEngine { namespace UI {

	const float UI_EPS = 0.0001f;
	const int UI_SUBDIVISIONS = 6;

	bool almostEqual(const float a, const float b) {
		return abs(a - b) < UI_EPS;
	}

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

		Math::Color4f c = color;
		//c.r = (float)(rand() % 500) / (float)500;

		*m_pVertex = { rect.position,                               c, Math::Vec2f(0, 0), 0 }; m_pVertex++; // 0
		*m_pVertex = { rect.position + Math::Vec2f(rect.width, 0),  c, Math::Vec2f(0, 0), 0 }; m_pVertex++; // 1
		*m_pVertex = { rect.position + Math::Vec2f(0, rect.height), c, Math::Vec2f(0, 0), 0 }; m_pVertex++; // 2
		*m_pVertex = { rect.position + rect.size,                   c, Math::Vec2f(0, 0), 0 }; m_pVertex++; // 3

		*m_pIndex = m_VertexCount + 0; m_pIndex++;
		*m_pIndex = m_VertexCount + 2; m_pIndex++;
		*m_pIndex = m_VertexCount + 1; m_pIndex++;
		*m_pIndex = m_VertexCount + 3; m_pIndex++;
		*m_pIndex = m_VertexCount + 1; m_pIndex++;
		*m_pIndex = m_VertexCount + 2; m_pIndex++;

		m_IndexCount += 6;
		m_VertexCount += 4;
	}

	void Painter::drawRoundedCorner(const Math::Vec2f& center, const Math::Vec2f& radii, const Corner corner, const Math::Color4f& color)
	{
		if (radii.x < UI_EPS || radii.y < UI_EPS) {
			// No radius, nothing to do
			return;
		}

		const int subdivisions = UI_SUBDIVISIONS;
		const float stepAngle = 0.5f * PI / (float)subdivisions; // quarter / subdivisions
		const float startAngle = 0.5f * PI * static_cast<int>(corner);

		// Top left reference
		//          A
		//        * |
		//     *    |
		//   *      | radii.y
		//  *       |
		// |--------C
		//   radii.x
		// 
		// C = center
		// A = first in chain
		// * = a offset

		int centerIndex = m_VertexCount;
		*m_pVertex = { center, color, Math::Vec2f(0, 0), 0 }; m_pVertex++; // C
		m_VertexCount += 1;

		for (int i = 0; i <= subdivisions; i++) {
			float angle = startAngle + i * stepAngle;
			Math::Vec2f offset = radii * Math::Vec2f(cos(angle), -sin(angle));

			*m_pVertex = { center + offset, color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
			m_VertexCount++;

			// first iteration is to create A
			if (i == 0)
				continue;

			*m_pIndex = m_VertexCount - 2; m_pIndex++;
			*m_pIndex = m_VertexCount - 1; m_pIndex++;
			*m_pIndex = centerIndex; m_pIndex++;
			m_IndexCount += 3;
		}
	}

	void Painter::drawRoundedCornerCarved(const Math::Vec2f& center, const Math::Vec2f& outerRadii, const Math::Vec2f& innerRadii, const Corner corner, const Math::Color4f& color)
	{
		if (innerRadii.x < UI_EPS || innerRadii.y < UI_EPS) {
			drawRoundedCorner(center, outerRadii, corner, color);
			return;
		}

		// See https://drafts.csswg.org/css-backgrounds-3/#corner-shaping

		// Top left reference
		//                         * outer.x
		//                    *    |
		//                *        | 
		//            *            |
		//         *               * inner.x
		//       *               *
		//     *               * 
		//   *               *
		//  * ------------- *      C (center)
		// outer.y       inner.y

		const int subdivisions = UI_SUBDIVISIONS;
		const float stepAngle = 0.5f * PI / (float)subdivisions; // quarter / subdivisions
		const float startAngle = 0.5f * PI * static_cast<int>(corner);

		for (int i = 0; i <= subdivisions; i++) {
			float angle = startAngle + i * stepAngle;

			Math::Vec2f offset(cos(angle), -sin(angle));
			Math::Vec2f out = outerRadii * offset;
			Math::Vec2f in = innerRadii * offset;

			*m_pVertex = { center + out, color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
			*m_pVertex = { center + in, color, Math::Vec2f(0, 0), 0 }; m_pVertex++;
			m_VertexCount += 2;

			// first iteration is to generate the initial two vertexs
			if (i == 0)
				continue;

			*m_pIndex = m_VertexCount - 2; m_pIndex++;
			*m_pIndex = m_VertexCount - 3; m_pIndex++;
			*m_pIndex = m_VertexCount - 4; m_pIndex++;
			*m_pIndex = m_VertexCount - 3; m_pIndex++;
			*m_pIndex = m_VertexCount - 2; m_pIndex++;
			*m_pIndex = m_VertexCount - 1; m_pIndex++;
			m_IndexCount += 6;
		}
	}

	void Painter::drawRoundedRectangle(const Math::Rectf& rect, const Math::Color4f& color, const RoundedRectParams& params)
	{
		const Math::Vec2f halfSize = rect.size * 0.5f;
		const Math::Vec2f& topLeftRadii = Math::Vec2f::Min(params.cornerRadii[0], halfSize);
		const Math::Vec2f& topRightRadii = Math::Vec2f::Min(params.cornerRadii[1], halfSize);
		const Math::Vec2f& bottomRightRadii = Math::Vec2f::Min(params.cornerRadii[2], halfSize);
		const Math::Vec2f& bottomLeftRadii = Math::Vec2f::Min(params.cornerRadii[3], halfSize);

		const float maxLeftRadii = std::max(topLeftRadii.x, bottomLeftRadii.x);
		const float minLeftRadii = std::min(topLeftRadii.x, bottomLeftRadii.x);
		const float maxRightRadii = std::max(topRightRadii.x, bottomRightRadii.x);
		const float minRightRadii = std::min(topRightRadii.x, bottomRightRadii.x);

		// corner centers
		//    __----------------__
		//   * \    |        |     *
		//  *    \  |        |     / *
		// *   TL \ |        |  / TR  *
		// |_______\|← B  A →|/_______|
		// |        |        |        |
		// |        |        |        |
		// |        |        |        |
		// |        |        |        |
		// |________|← C  D →|________|
		// *       /|        |\       *
		//  * BL /  |        |  \BR  *
		//    * /   |        |   \ *
		//     *____|________|__ *
		const Math::Vec2f topRightCorner = rect.position + Math::Vec2f(rect.size.x - topRightRadii.x, topRightRadii.y); // A
		const Math::Vec2f topLeftCorner = rect.position + topLeftRadii; // B
		const Math::Vec2f bottomLeftCorner = rect.position + Math::Vec2f(bottomLeftRadii.x, rect.size.y - bottomLeftRadii.y); // C
		const Math::Vec2f bottomRightCorner = rect.position + rect.size - bottomRightRadii; // D

		drawRoundedCorner(topRightCorner, topRightRadii, Corner::TOP_RIGHT, color);
		drawRoundedCorner(topLeftCorner, topLeftRadii, Corner::TOP_LEFT, color);
		drawRoundedCorner(bottomLeftCorner, bottomLeftRadii, Corner::BOTTOM_LEFT, color);
		drawRoundedCorner(bottomRightCorner, bottomRightRadii, Corner::BOTTOM_RIGHT, color);

		// (1) both corners have the same radii.x
		//    __--------------
		//   * \    |        |
		//  *    \  |        |
		// *   TL \ |        |
		// |_______\|        |
		// |        |        |
		// |        |        |
		// |   A    | CENTER |
		// |        |        |
		// |________|        |
		// *       /|        |
		//  * BL /  |        |
		//    * /   |        |
		//     *____|________|

		// (2) top radii.x > bottom radii.x
		//    __--------------
		//   * \    |        |
		//  *    \  |        |
		// *   TL \ |        |
		// |_______\|        |
		// |    |   |        |
		// |    |   |        |
		// | A  |   | CENTER |
		// |    |   |        |
		// |____| B |        |
		// * BL/|   |        |
		//  */  |   |        |
		//    *_|___|________|

		// (3) bottom radii.x > top radii.x
		//    __--------------
		//   * |    |        |
		//  *  |    |        |
		// * TL|    |        |
		// |___|    |        |
		// |   |  B |        |
		// |   |    |        |
		// | A |    | CENTER |
		// |   |    |        |
		// |___|____|        |
		// *       /|        |
		//  * BL /  |        |
		//    * /   |        |
		//     *____|________|

		// center
		drawRectangle(Math::Rectf(rect.x + maxLeftRadii, rect.y, rect.width - maxLeftRadii - maxRightRadii, rect.height), color);

		// ---------- left side ----------

		// A (is the same for 1, 2, 3)
		drawRectangle(Math::Rectf(rect.x, rect.y + topLeftRadii.y, minLeftRadii, rect.height - topLeftRadii.y - bottomLeftRadii.y), color);

		if (almostEqual(topLeftRadii.x, bottomLeftRadii.x)) { } // (1)
		else if(topLeftRadii.x > bottomLeftRadii.x) { // (2)
			// B
			drawRectangle(Math::Rectf(rect.x + minLeftRadii, rect.y + topLeftRadii.y, maxLeftRadii - minLeftRadii, rect.height - topLeftRadii.y), color);
		}
		else { // (3)
			// B
			drawRectangle(Math::Rectf(rect.x + minLeftRadii, rect.y, maxLeftRadii - minLeftRadii, rect.height - bottomLeftRadii.y), color);
		}

		// ---------- right side ----------

		// A (is the same for 1, 2, 3)
		drawRectangle(Math::Rectf(rect.x + rect.width - minRightRadii, rect.y + topRightRadii.y, minRightRadii, rect.height - topRightRadii.y - bottomRightRadii.y), color);

		if (almostEqual(topRightRadii.x, bottomRightRadii.x)) {} // (1)
		else if (topRightRadii.x > bottomRightRadii.x) { // (2)
			// B
			drawRectangle(Math::Rectf(rect.x + rect.width - maxRightRadii, rect.y + topRightRadii.y, maxRightRadii - minRightRadii, rect.height - topRightRadii.y), color);
		}
		else { // (3)
			// B
			drawRectangle(Math::Rectf(rect.x + rect.width - maxRightRadii, rect.y, maxRightRadii - minRightRadii, rect.height - bottomRightRadii.y), color);
		}
	}

	void Painter::drawRoundedBorders(const Math::Rectf& rect, const Math::Color4f& color, const RoundedBordersParams& params)
	{
		//    __--------------__   
		//   *  |            |  *  
		//  *   |      A     |   * 
		// *____|____________|____*
		// |    |            |    |
		// | B  |            |  D |
		// |____|____________|____|
		// *    |            |    *
		//  *   |      C     |   * 
		//   *__|____________|__* 
		
		const Math::Vec2f halfSize = rect.size * 0.5f;
		const Math::Vec2f& topLeftRadii     = Math::Vec2f::Min(params.rectParams.cornerRadii[0], halfSize);
		const Math::Vec2f& topRightRadii    = Math::Vec2f::Min(params.rectParams.cornerRadii[1], halfSize);
		const Math::Vec2f& bottomRightRadii = Math::Vec2f::Min(params.rectParams.cornerRadii[2], halfSize);
		const Math::Vec2f& bottomLeftRadii  = Math::Vec2f::Min(params.rectParams.cornerRadii[3], halfSize);

		const float leftWidth   = params.widths[0];
		const float topWidth    = params.widths[1];
		const float rightWidth  = params.widths[2];
		const float bottomWidth = params.widths[3];

		// A
		drawRectangle(Math::Rectf(rect.x + topLeftRadii.x, rect.y, rect.width - topLeftRadii.x - topRightRadii.x, topWidth), color);
		// C
		drawRectangle(Math::Rectf(rect.x + bottomLeftRadii.x, rect.y + rect.height - bottomWidth, rect.width - bottomLeftRadii.x - bottomRightRadii.x, bottomWidth), color);
		// B
		drawRectangle(Math::Rectf(rect.x, rect.y + topLeftRadii.y, leftWidth, rect.height - topLeftRadii.y - bottomLeftRadii.y), color);
		// D
		drawRectangle(Math::Rectf(rect.x + rect.width - rightWidth, rect.y + topRightRadii.y, rightWidth, rect.height - topRightRadii.y - bottomRightRadii.y), color);
		
		/*
			From https://drafts.csswg.org/css-backgrounds-3/#corner-shaping:

			The padding edge (inner border) radius is the outer border radius minus the corresponding border thickness.
			In the case where this results in a negative value, the inner radius is zero.

			outer edge = cornerRadii
			inner edge = cornerRadii - sidesBorderWidths

			if inner edge (x or y) < 0 then inner edge = 0,0
			(handled in drawRoundedCornerCarved, which fall back to drawRoundedCorner)
		*/

		const Math::Vec2f topRightCorner = rect.position + Math::Vec2f(rect.size.x - topRightRadii.x, topRightRadii.y); // A
		const Math::Vec2f topLeftCorner = rect.position + topLeftRadii; // B
		const Math::Vec2f bottomLeftCorner = rect.position + Math::Vec2f(bottomLeftRadii.x, rect.size.y - bottomLeftRadii.y); // C
		const Math::Vec2f bottomRightCorner = rect.position + rect.size - bottomRightRadii; // D

		const Math::Vec2f topLeftInnerRadii = topLeftRadii - Math::Vec2f(leftWidth, topWidth);
		const Math::Vec2f topRightInnerRadii = topRightRadii - Math::Vec2f(rightWidth, topWidth);
		const Math::Vec2f bottomLeftInnerRadii = bottomLeftRadii - Math::Vec2f(leftWidth, bottomWidth);
		const Math::Vec2f bottomRightInnerRadii = bottomRightRadii - Math::Vec2f(rightWidth, bottomWidth);

		drawRoundedCornerCarved(topLeftCorner, topLeftRadii, topLeftInnerRadii, Corner::TOP_LEFT, color);
		drawRoundedCornerCarved(topRightCorner, topRightRadii, topRightInnerRadii, Corner::TOP_RIGHT, color);
		drawRoundedCornerCarved(bottomLeftCorner, bottomLeftRadii, bottomLeftInnerRadii, Corner::BOTTOM_LEFT, color);
		drawRoundedCornerCarved(bottomRightCorner, bottomRightRadii, bottomRightInnerRadii, Corner::BOTTOM_RIGHT, color);

		// TODO: make rectangles where radius < border width
		//       i.e. bottomLeftInnerRadii.y (bottomRightRadii.y - bottomWidth) < 0 
		// *******************
		// |  |            |
		// |  |   CENTER   |
		// |LW|    ________|
		// |  |XXX|        |
		// |  |XXX|        |
		// |__|___|   BW   |
		//  *     |        |
		//   *    |        |
		//    *___|________|
	}

	void Painter::drawText(const Graphics::TextLayout& textLayout, const Math::Vec2f& position, const Math::Color4f& color)
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

			*m_pIndex = m_VertexCount + 0; m_pIndex++;
			*m_pIndex = m_VertexCount + 1; m_pIndex++;
			*m_pIndex = m_VertexCount + 2; m_pIndex++;
			*m_pIndex = m_VertexCount + 2; m_pIndex++;
			*m_pIndex = m_VertexCount + 3; m_pIndex++;
			*m_pIndex = m_VertexCount + 0; m_pIndex++;

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
