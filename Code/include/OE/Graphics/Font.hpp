#ifndef GRAPHICS_FONT_HPP
#define GRAPHICS_FONT_HPP

#include <string>
#include <vector>

#include "FontInstance.hpp"

/* Forward defs */
struct FT_LibraryRec_;
typedef FT_LibraryRec_* FT_Library;
struct FT_FaceRec_;
typedef FT_FaceRec_* FT_Face;

namespace OrbitEngine { namespace Graphics {

	const std::wstring DEFAULT_FONT_CHARSET = L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	/// Represents a single typeface
	class Font {
	public:
		static FT_Library s_FTLibrary;

		/// Load a font file
		Font(const std::string& file);
		~Font();

		std::wstring getFullCharset() const;

		/**
			Generate a new FontInstance given a size, a charset and an atlas mode

			@param[in] size target size in pixels
			@param[in] charset characters requested for this instance
			@param[in] mode kind of font atlas to generate
		*/
		FontInstance* generateInstance(FontSize size, FontAtlasMode mode, const std::wstring& charset = DEFAULT_FONT_CHARSET);
		FontInstance* loadInstance(FontSize size, FontAtlasMode mode, const std::wstring& charset = DEFAULT_FONT_CHARSET);

	private:
		std::vector<char> m_FileBuffer;
		FT_Face m_Face;

		/// Create a FontInstance object and fill all the relevant glyph and kerning data
		FontInstance* initInstance(FontSize size, const std::wstring& charset);
	};
} }

#endif