#ifndef GRAPHICS_FONT_HPP
#define GRAPHICS_FONT_HPP

#include <map>

#include "OE/Misc/RectsPacker.hpp"

#include "OE/Math/Vec2.hpp"
#include "OE/Math/UV.hpp"

#include "OE/Graphics/API/Texture.hpp"

#define OE_FONT_START ' '
#define OE_FONT_END   '~'
#define OE_FONT_COUNT (OE_FONT_END - OE_FONT_START)

/* Forward defs */
struct FT_LibraryRec_;
typedef FT_LibraryRec_* FT_Library;
struct FT_FaceRec_;
typedef FT_FaceRec_* FT_Face;

namespace OrbitEngine { namespace Graphics {
	class SpriteRenderer;
	class Texture;

	struct Glyph : public Misc::Packeable2D {
		char c;
		Math::Vec2f m_Size;
		Math::UV m_UVs;

		/* https://www.freetype.org/freetype2/docs/glyphs/metrics.png */
		int m_H_advance, m_H_bearingX, m_H_bearingY;
		int m_V_advance, m_V_bearingX, m_V_bearingY;
	};

	enum class FontStyle {
		NORMAL,
		BOLD,
		ITALIC,
		BOLD_AND_ITALIC
	};

	struct FontInstance {
	public:
		FontInstance() {
			m_Glyphs = std::vector<Glyph>(OE_FONT_COUNT + 1);
			m_Kernings = std::map<std::pair<unsigned char, unsigned char>, float>();
		}
		~FontInstance() {
			delete m_Texture;
			m_Glyphs.clear();
			m_Kernings.clear();
		}

		float m_Size;
		float m_MaxHeight;
		float m_MaxBearing;

		Texture* m_Texture;
		std::vector<Glyph> m_Glyphs;
		std::map<std::pair<unsigned char, unsigned char>, float> m_Kernings;
	};

	class Font {
	public:
		Font(std::string file);
		~Font();

		void prepareFont(float size); // TODO: Add effects
		void drawString(const std::string& text, const Math::Vec2f& position, SpriteRenderer& r2d);
		float getWidth(const std::string& text);
		float getHeight(const std::string& text);
		Math::Vec2f getBounds(const std::string& text);
		float getHorizontalKerning(char left, char right);

		FontInstance* getInstance() const { return m_Instance; }

		static FT_Library s_FTLibrary;
		static Font* GetDefaultFont();
	private:
		static Font* s_DefaultFont;

		std::vector<char> m_FileBuffer;
		FT_Face m_Face;

		std::vector<FontInstance*> m_FontInstances;
		FontInstance* m_Instance;
	};
} }

#endif