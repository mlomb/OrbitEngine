#ifndef GRAPHICS_SPRITE_RENDERER_HPP
#define GRAPHICS_SPRITE_RENDERER_HPP

#include "Renderer2D.hpp"

#include "OE/Math/Vec2.hpp"
#include "OE/Math/UV.hpp"
#include "OE/Math/Color.hpp"

namespace OrbitEngine { namespace Graphics {
	class Texture;

	class SpriteRenderer : virtual public Renderer2D {
	public:
		void bindColor(const Math::Color& color);
		void bindTexture(Texture* texture);

		void rect(const Math::Vec2f& position, const Math::Vec2f& size);
		void rect(const Math::Vec2f& position, const Math::Vec2f& size, const Math::UV& uvs, bool flipX = false, bool flipY = false);

	protected:
		virtual void begin();
		virtual void end();
		virtual int handleTexture(Texture* tex) = 0;
		virtual void submitRect(const Math::Vec2f(&positions)[4], const Math::Vec2f(&uvs)[4], const Math::Color& color, unsigned int texture) = 0;

	private:
		Math::Color m_BindColor;
		unsigned int m_BindTexture;

		// Little cache
		Math::Vec2f m_TempVertexs[4];
		Math::Vec2f m_TempUVs[4];
	};
} }

#endif