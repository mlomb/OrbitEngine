#ifndef MISC_RECTS_PACKER_HPP
#define MISC_RECTS_PACKER_HPP

/*
This is a modified version of this algorithm:
http://blackpawn.com/texts/lightmaps/default.html
*/

#include <vector>

#include "OE/Math/Vec2.hpp"

namespace OrbitEngine { namespace Misc {
	struct Packeable2DRectLTRB;
	struct Packeable2DRectXYWH;

	struct Packeable2DRectWH {
		Packeable2DRectWH(const Packeable2DRectLTRB&);
		Packeable2DRectWH(const Packeable2DRectXYWH&);
		Packeable2DRectWH(int w = 0, int h = 0);
		int w, h, area(), perimeter(), fits(const Packeable2DRectWH& bigger) const; // 0 - no, 1 - yes, 2 - flipped, 3 - perfectly, 4 perfectly flipped
	};

	struct Packeable2DRectLTRB {
		Packeable2DRectLTRB();
		Packeable2DRectLTRB(int left, int top, int right, int bottom);
		int l, t, r, b, w() const, h() const, area() const, perimeter() const;
		void w(int), h(int);
	};

	struct Packeable2DRectXYWH : public Packeable2DRectWH {
		Packeable2DRectXYWH();
		Packeable2DRectXYWH(const Packeable2DRectLTRB&);
		Packeable2DRectXYWH(int x, int y, int width, int height);
		operator Packeable2DRectLTRB();

		int x, y, r() const, b() const;
		void r(int), b(int);
	};

	struct Packeable2D : public Packeable2DRectXYWH {
		Packeable2D(const Packeable2DRectLTRB&);
		Packeable2D(int x, int y, int width, int height);
		Packeable2D();
		void flip();
		bool flipped;
	};

	class Packer2D {
	public:
		static bool Pack(std::vector<Packeable2D*>& rects, int max_side, Math::Vec2f& out_size, int discardStep = 1);
	};
} }

#endif