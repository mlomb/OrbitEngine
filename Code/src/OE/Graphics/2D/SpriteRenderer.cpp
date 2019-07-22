#include "OE/Graphics/2D/SpriteRenderer.hpp"

namespace OrbitEngine { namespace Graphics {

	void SpriteRenderer::bindColor(const Math::Color& color)
	{
		m_BindColor = color;
	}

	void SpriteRenderer::bindTexture(Texture* texture)
	{
		m_BindTexture = handleTexture(texture);
	}

	void SpriteRenderer::rect(const Math::Vec2f& position, const Math::Vec2f& size)
	{
		rect(position, size, Math::UV_Default);
	}

	void SpriteRenderer::rect(const Math::Vec2f& position, const Math::Vec2f& size, const Math::UV& uvs, bool flipX, bool flipY)
	{
		m_TempVertexs[0].x = position.x;
		m_TempVertexs[0].y = position.y;
		m_TempVertexs[1].x = position.x;
		m_TempVertexs[1].y = position.y + size.y;
		m_TempVertexs[2].x = position.x + size.x;
		m_TempVertexs[2].y = position.y + size.y;
		m_TempVertexs[3].x = position.x + size.x;
		m_TempVertexs[3].y = position.y;
		/*
		if (m_MatrixStack->countStack() > 0) { // Skip the multiplication to the identity
			Mat4& mat = m_MatrixStack->get();
			m_TempVertexs[0] = mat.multiply(m_TempVertexs[0]);
			m_TempVertexs[1] = mat.multiply(m_TempVertexs[1]);
			m_TempVertexs[2] = mat.multiply(m_TempVertexs[2]);
			m_TempVertexs[3] = mat.multiply(m_TempVertexs[3]);
		}
		*/
		if (flipX) {
			// TODO See WRONG
			m_TempUVs[0] = uvs._4;
			m_TempUVs[1] = uvs._3;
			m_TempUVs[2] = uvs._2;
			m_TempUVs[3] = uvs._1;
		}
		else if (flipY) {
			// TODO
			OE_LOG_WARNING("TODO FlipY!");
		}
		else if (flipX && flipY) {
			// TODO
			OE_LOG_WARNING("TODO FlipX && FlipY!");
		}
		else {
			m_TempUVs[0] = uvs._1;
			m_TempUVs[1] = uvs._2;
			m_TempUVs[2] = uvs._3;
			m_TempUVs[3] = uvs._4;
		}

		submitRect(m_TempVertexs, m_TempUVs, m_BindColor, m_BindTexture);
	}

	void SpriteRenderer::begin()
	{
		// Renderer2D::begin must be called by the derived class

		m_BindColor = Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
		m_BindTexture = 0;
	}

	void SpriteRenderer::end()
	{
		// Renderer2D::end must be called by the derived class

	}

} }