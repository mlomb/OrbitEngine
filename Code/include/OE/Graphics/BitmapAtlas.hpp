#ifndef GRAPHICS_BITMAP_ATLAS_HPP
#define GRAPHICS_BITMAP_ATLAS_HPP

#include <map>

#include "Atlas.hpp"
#include "Bitmap.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Misc/RectsPacker.hpp"

namespace OrbitEngine { namespace Graphics {
	/**
		@brief A 2D bitmap atlas

		Example: **manually** make an atlas of four 64x64 bitmaps
		@code
		BitmapAtlasRGBA* atlas = new BitmapAtlasRGBA(128, 128);
		atlas->addFrame(0, img1,  0,  0, false);
		atlas->addFrame(1, img2, 64,  0, false);
		atlas->addFrame(2, img3,  0, 64, false);
		atlas->addFrame(3, img4, 64, 64, false);
		atlas->exportToFiles("atlas.json", "atlas.png");
		delete atlas;
		@endcode

		Example: **automagically** make an atlas out of bitmaps
		@code
		std::map<FrameIndex, BitmapRGBA> bitmaps;
		bitmaps.emplace(0, img1);
		bitmaps.emplace(1, img2);
		bitmaps.emplace(2, img3);
		bitmaps.emplace(3, img4);
		BitmapAtlasRGBA* atlas = BitmapAtlasRGBA::Generate(bitmaps, 2048);
		if (atlas) {
			atlas->exportToFiles("atlas.json", "atlas.png");
			delete atlas;
		}
		@endcode

		Currently, its not possible to load a BitmapAtlas from disk.
		Take a look into TextureAtlas for using it during runtime.
	*/
	template<typename T, unsigned int N = 1>
	class BitmapAtlas : public Atlas, protected Bitmap<T, N> {
	public:
		/// Creates an empty WxH bitmap atlas
		BitmapAtlas(unsigned int w, unsigned int h);
		~BitmapAtlas();

		Math::Vec2f getTexelSize() const override;

		/**
			@brief Add a bitmap frame at \p x, \p y
			@param[in] index identifier within the atlas, must be unique
			@param[in] bitmap Bitmap to write
			@param[in] x, y location to write the bitmap
			@param[in] flip if the input bitmap must be rotated 90 degrees clockwise
			@param[in] meta additional metadata
			@return Whether the frame was added correctly
			@note The operation may fail if the bitmap is invalid, the coordinates are out of bounds or
				  the index provided was already in use
		*/
		bool addFrame(FrameIndex index, const Bitmap<T, N>& bitmap, int x, int y, bool flip, FrameMetadata meta);

		/**
			@brief Export the bitmap atlas to disk
			@param[in] metadata path to the metadata file to write (.json)
			@param[in] image path to the image file to write (.png)
			@return Whether the atlas was exported correctly
			@see Atlas for the atlas's metadata format
		*/
		bool exportToFiles(const std::string& metadata, const std::string& image) const;

		/**
			@brief Generate an atlas from bitmaps
			@param[in] bitmaps map containing the desired indexes mapped to their corresponding bitmaps and metadata
			@param[in] max_size maximum allowed size for the generated atlas. The generation may fail if its not possible to fit the bitmaps
			@param[in] padding separation in pixels between each bitmap in the atlas
			@param[in] background color of the unused pixels
			@param[in] POT force the atlas generation to be a power of two
			@return The BitmapAtlas instance or NULL if the operation failed
		*/
		static BitmapAtlas<T, N>* Generate(const std::map<FrameIndex, std::pair<Bitmap<T, N>, FrameMetadata>>& bitmaps, unsigned int max_size, unsigned int padding = 0, const T background[N] = 0, bool POT = false);
	};

	typedef BitmapAtlas<unsigned char, 3> BitmapAtlasRGB;
	typedef BitmapAtlas<unsigned char, 4> BitmapAtlasRGBA;

	template<typename T, unsigned int N> BitmapAtlas<T, N>::BitmapAtlas(unsigned int w, unsigned int h)
		: Bitmap<T, N>(w, h), Atlas()
	{
	}

	template<typename T, unsigned int N> BitmapAtlas<T, N>::~BitmapAtlas() {
	}

	template<typename T, unsigned int N> bool BitmapAtlas<T, N>::addFrame(FrameIndex index, const Bitmap<T, N>& bitmap, int x, int y, bool flip, FrameMetadata meta) {
		if (!bitmap.valid()) // bitmap is invalid
			return false;
		if (x < 0 || y < 0 || x > Bitmap<T, N>::m_Width || y > Bitmap<T, N>::m_Height) // out of bounds
			return false;
		if (!Atlas::addFrame(index, x, y, flip ? bitmap.height() : bitmap.width(), flip ? bitmap.width() : bitmap.height(), flip, meta))
			return false;
		// next line shouldn't fail
		Bitmap<T, N>::write(flip ? bitmap.rotate90clockwise() : bitmap, x, y);
		return true;
	}

	template<typename T, unsigned int N> bool BitmapAtlas<T, N>::exportToFiles(const std::string& metadata, const std::string& image) const {
		return Bitmap<T, N>::savePNG(image) && Atlas::exportToFile(metadata);
	}

	template<typename T, unsigned int N> Math::Vec2f BitmapAtlas<T, N>::getTexelSize() const {
		return Math::Vec2f(1.0f / Bitmap<T, N>::m_Width, 1.0f / Bitmap<T, N>::m_Height);
	}

	template<typename T, unsigned int N> BitmapAtlas<T, N>* BitmapAtlas<T, N>::Generate(const std::map<FrameIndex, std::pair<Bitmap<T, N>, FrameMetadata>>& bitmaps, unsigned int max_size, unsigned int padding, const T background[N], bool POT) {
		std::vector<Misc::Packeable2D*> rects;

		struct Entry : public Misc::Packeable2D {
			Entry(int index, const Bitmap<T, N>& bitmap, FrameMetadata metadata, unsigned int padding)
				: Misc::Packeable2D(), index(index), bitmap(bitmap), metadata(metadata) { w = bitmap.width() + padding; h = bitmap.height() + padding; }
			const int index;
			const Bitmap<T, N>& bitmap;
			const FrameMetadata metadata;
		};

		rects.reserve(bitmaps.size());
		for (auto& p : bitmaps) {
			rects.push_back(new Entry(p.first, p.second.first, p.second.second, padding));
		}

		unsigned int atlas_width, atlas_height;
		if (!Misc::Packer2D::Pack(rects, max_size, atlas_width, atlas_height)) {
			OE_LOG_WARNING("BitmapAtlas: packing was not possible");
			return nullptr;
		}

		if (POT) {
			unsigned int size = std::max(Math::nextPowerOfTwo(atlas_width), Math::nextPowerOfTwo(atlas_height));
			atlas_width = atlas_height = size;
		}

		// TODO: Rethink padding strategy
		if (!POT) {
			atlas_width -= padding;
			atlas_height -= padding;
		}

		BitmapAtlas<T, N>* atlas = new BitmapAtlas<T, N>(atlas_width, atlas_height);
		if (background == NULL)
			atlas->clear();
		else
			atlas->fill(background);

		for (Misc::Packeable2D* p : rects) {
			Entry* e = static_cast<Entry*>(p);
			atlas->addFrame(e->index, e->bitmap, e->x, e->y, e->flipped, e->metadata);
			delete e;
		}

		rects.clear();

		return atlas;
	}
} }

#endif