#ifndef GRAPHICS_BITMAP_ATLAS_HPP
#define GRAPHICS_BITMAP_ATLAS_HPP

#include <map>

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

		bool addFrame(FrameIndex index, const Bitmap<T, N>& bitmap, int x, int y, bool flip);

		/**
			@brief Generate an atlas of bitmaps
			@note The frame indices will match the input vector indices
			@return The BitmapAtlas instance or NULL if the operation failed
		*/
		static BitmapAtlas<T, N>* Generate(std::map<FrameIndex, Bitmap<T, N>>& bitmaps, unsigned int max_size, unsigned int padding = 0);

	protected:

		float getTexelSize() override;
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

	template<typename T, unsigned int N> bool BitmapAtlas<T, N>::addFrame(FrameIndex index, const Bitmap<T, N>& bitmap, int x, int y, bool flip)
	{
		write(flip ? bitmap.rotate90clockwise() : bitmap, x, y);
		return Atlas::addFrame(index, x, y, flip ? bitmap.height() : bitmap.width(), flip ? bitmap.width() : bitmap.height(), flip);
	}

	template<typename T, unsigned int N> BitmapAtlas<T, N>* BitmapAtlas<T, N>::Generate(std::map<FrameIndex, Bitmap<T, N>>& bitmaps, unsigned int max_size, unsigned int padding)
	{
		std::vector<Misc::Packeable2D*> rects;

		struct Entry : public Misc::Packeable2D {
			Entry(int index, const Bitmap<T, N>& bitmap, unsigned int padding)
				: Misc::Packeable2D(), index(index), bitmap(bitmap) { w = bitmap.width() + padding; h = bitmap.height() + padding; }
			const int index;
			const Bitmap<T, N>& bitmap;
		};

		rects.reserve(bitmaps.size());
		for (auto& p : bitmaps) {
			rects.push_back(new Entry(p.first, p.second, padding));
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
			atlas->addFrame(e->index, e->bitmap, e->x, e->y, e->flipped);
			delete e;
		}

		rects.clear();

		return atlas;
	}

	template<typename T, unsigned int N> float BitmapAtlas<T, N>::getTexelSize()
	{
		return 1.0f / m_Width;
	}
} }

#endif