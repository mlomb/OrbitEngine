#ifndef GRAPHICS_BITMAP_ATLAS_HPP
#define GRAPHICS_BITMAP_ATLAS_HPP

#include "Atlas.hpp"
#include "Bitmap.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Misc/RectsPacker.hpp"

namespace OrbitEngine { namespace Graphics {
	/// A 2D bitmap atlas
	template<typename T, unsigned int N = 1>
	class BitmapAtlas : public Bitmap<T, N>, public Atlas {
	public:
		/// Creates an empty WxH atlas
		BitmapAtlas(unsigned int w, unsigned int h);
		~BitmapAtlas();

		int addFrame(const std::string& name, const Bitmap<T, N>& bitmap, int x, int y);

		/**
			Generate a BitmapAtlas
			@return The BitmapAtlas instance or NULL if the operation failed
		*/
		static BitmapAtlas<T, N>* Generate(std::vector<std::pair<std::string, Bitmap<T, N>>>& bitmaps, unsigned int max_size, unsigned int padding = 0);
	};

	typedef BitmapAtlas<unsigned char, 3> BitmapAtlasRGB;
	typedef BitmapAtlas<unsigned char, 4> BitmapAtlasRGBA;

	template<typename T, unsigned int N> BitmapAtlas<T, N>::BitmapAtlas(unsigned int w, unsigned int h)
		: Bitmap<T, N>(w, h), Atlas()
	{
	}

	template<typename T, unsigned int N> BitmapAtlas<T, N>::~BitmapAtlas()
	{
	}

	template<typename T, unsigned int N> int BitmapAtlas<T, N>::addFrame(const std::string& name, const Bitmap<T, N>& bitmap, int x, int y)
	{
		write(bitmap, x, y);
		return Atlas::addFrame(name, { Math::UV() });
	}

	template<typename T, unsigned int N> BitmapAtlas<T, N>* BitmapAtlas<T, N>::Generate(std::vector<std::pair<std::string, Bitmap<T, N>>>& bitmaps, unsigned int max_size, unsigned int padding)
	{
		std::vector<Misc::Packeable2D*> rects;

		struct Entry : public Misc::Packeable2D {
			Entry(const std::string& name, const Bitmap<T, N>& bitmap, unsigned int padding)
				: Misc::Packeable2D(), name(name), bitmap(bitmap) { w = bitmap.width() + padding; h = bitmap.height() + padding; }
			const std::string& name;
			const Bitmap<T, N>& bitmap;
		};

		rects.reserve(bitmaps.size());
		for (auto& pair : bitmaps) {
			rects.push_back(new Entry(pair.first, pair.second, padding));
		}

		unsigned int atlas_width, atlas_height;
		if (!Misc::Packer2D::Pack(rects, max_size, atlas_width, atlas_height)) {
			OE_LOG_WARNING("BitmapAtlas: packing was not possible");
			return nullptr;
		}

		// TODO: Rethink padding strategy
		if (!Math::isPowerOfTwo(atlas_width) && !Math::isPowerOfTwo(atlas_height)) {
			atlas_width -= padding;
			atlas_height -= padding;
		}

		BitmapAtlas<T, N>* atlas = new BitmapAtlas<T, N>(atlas_width, atlas_height);
		atlas->clear();

		for (Misc::Packeable2D* p : rects) {
			Entry* e = static_cast<Entry*>(p);
			if(e->flipped)
				atlas->addFrame(e->name, e->bitmap.rotate90clockwise(), e->x, e->y);
			else
				atlas->addFrame(e->name, e->bitmap, e->x, e->y);
			delete e;
		}

		rects.clear();

		return atlas;
	}
} }

#endif