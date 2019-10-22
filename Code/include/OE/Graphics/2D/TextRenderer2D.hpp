#ifndef GRAPHICS_TEXT_RENDERER2D_HPP
#define GRAPHICS_TEXT_RENDERER2D_HPP

#include "OE/Graphics/2D/BatchRenderer2D.hpp"
#include "OE/Graphics/2D/TextureArrayHandler.hpp"
#include "OE/Graphics/FontCollection.hpp"

namespace OrbitEngine { namespace Graphics {
	struct TextRenderer2DVertex {
		Math::Vec2f position;
		unsigned int color;
		unsigned int borderColor;
		Math::Vec2f uv;
		float tid;
		float mode;
		float border; // [0..1]
	};

	struct TextStyle {
		FontSize size = 20.0f;
		unsigned int color = 0xFFFFFFFF;
		unsigned int borderColor = 0x0;
		float border = 0.0f;
		// TODO: Bold, glow, etc
	};

	class TextRenderer2D : public BatchRenderer2D<TextRenderer2DVertex>, private TextureArrayHandler {
	public:
		TextRenderer2D(unsigned int batchSize = 4000);
		~TextRenderer2D();

		void begin() override;
		void end() override;

		void drawText(const std::vector<GlyphCodepoint>& text, const Math::Vec2f& position, TextStyle style, FontCollection* font);
		Math::Vec2f getBounds(const std::vector<GlyphCodepoint>& text, TextStyle style, FontCollection* font);

	private:
		Shader* m_Shader;
		unsigned int m_IndexCount;
	};
} }

#endif