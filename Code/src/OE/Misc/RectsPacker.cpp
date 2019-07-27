#include "OE/Misc/RectsPacker.hpp"

/*
This is a modified version of this algorithm:
http://blackpawn.com/texts/lightmaps/default.html
*/

#include "OE/Math/Math.hpp"

namespace OrbitEngine { namespace Misc {
	bool area(Packeable2D* a, Packeable2D* b) {
		return a->area() > b->area();
	}

	bool perimeter(Packeable2D* a, Packeable2D* b) {
		return a->perimeter() > b->perimeter();
	}

	bool max_side(Packeable2D* a, Packeable2D* b) {
		return std::max(a->w, a->h) > std::max(b->w, b->h);
	}

	bool max_width(Packeable2D* a, Packeable2D* b) {
		return a->w > b->w;
	}

	bool max_height(Packeable2D* a, Packeable2D* b) {
		return a->h > b->h;
	}

	bool(*cmpf[])(Packeable2D*, Packeable2D*) = {
		area,
		perimeter,
		max_side,
		max_width,
		max_height
	};

	struct Packeable2DNode {
		struct pnode {
			Packeable2DNode* pn;
			bool fill;

			pnode() : fill(false), pn(0) {}
			void set(int l, int t, int r, int b) {
				if (!pn) pn = new Packeable2DNode(Packeable2DRectLTRB(l, t, r, b));
				else {
					(*pn).rc = Packeable2DRectLTRB(l, t, r, b);
					(*pn).id = false;
				}
				fill = true;
			}
		};

		pnode c[2];
		Packeable2DRectLTRB rc;
		bool id;
		Packeable2DNode(Packeable2DRectLTRB rc = Packeable2DRectLTRB()) : id(false), rc(rc) {}

		void reset(const Packeable2DRectWH& r) {
			id = false;
			rc = Packeable2DRectLTRB(0, 0, r.w, r.h);
			delcheck();
		}

		Packeable2DNode* insert(Packeable2D& img) {
			if (c[0].pn && c[0].fill) {
				Packeable2DNode* newn;
				if (newn = c[0].pn->insert(img)) return newn;
				return    c[1].pn->insert(img);
			}

			if (id) return 0;
			int f = img.fits(Packeable2DRectXYWH(rc));

			switch (f) {
			case 0: return 0;
			case 1: img.flipped = false; break;
			case 2: img.flipped = true; break;
			case 3: id = true; img.flipped = false; return this;
			case 4: id = true; img.flipped = true;  return this;
			}

			int iw = (img.flipped ? img.h : img.w), ih = (img.flipped ? img.w : img.h);

			if (rc.w() - iw > rc.h() - ih) {
				c[0].set(rc.l, rc.t, rc.l + iw, rc.b);
				c[1].set(rc.l + iw, rc.t, rc.r, rc.b);
			}
			else {
				c[0].set(rc.l, rc.t, rc.r, rc.t + ih);
				c[1].set(rc.l, rc.t + ih, rc.r, rc.b);
			}

			return c[0].pn->insert(img);
		}

		void delcheck() {
			if (c[0].pn) { c[0].fill = false; c[0].pn->delcheck(); }
			if (c[1].pn) { c[1].fill = false; c[1].pn->delcheck(); }
		}

		~Packeable2DNode() {
			if (c[0].pn) delete c[0].pn;
			if (c[1].pn) delete c[1].pn;
		}
	};


	bool Packer2D::Pack(std::vector<Packeable2D*>& rects, int max_side, unsigned int& out_width, unsigned int& out_height, int discard_step) {
		Packeable2DRectWH size(max_side, max_side);

		for (Packeable2D* p : rects)
			if (!p->fits(size)) return false;

		Packeable2DNode root;
		const int funcs = (sizeof(cmpf) / sizeof(bool(*)(Packeable2D*, Packeable2D*)));

		std::vector<Packeable2D*> order[funcs];

		for (int f = 0; f < funcs; ++f) {
			order[f] = std::vector<Packeable2D*>(rects);
			std::sort(order[f].begin(), order[f].end(), cmpf[f]);
		}

		int min_func = -1, best_func = 0, best_area = 0, _area = 0, step, fit;
		size_t i;
		bool fail = false;

		for (int f = 0; f < funcs; ++f) {
			rects = order[f];
			step = size.w / 2;
			root.reset(size);

			while (true) {
				if (root.rc.w() > size.w) {
					if (min_func > -1) break;
					_area = 0;

					root.reset(size);
					for (i = 0; i < rects.size(); ++i)
						if (root.insert(*rects[i]))
							_area += rects[i]->area();

					fail = true;
					break;
				}

				fit = -1;

				for (i = 0; i < rects.size(); ++i)
					if (!root.insert(*rects[i])) {
						fit = 1;
						break;
					}

				if (fit == -1 && step <= discard_step)
					break;

				root.reset(Packeable2DRectWH(root.rc.w() + fit*step, root.rc.h() + fit*step));

				step /= 2;
				if (!step)
					step = 1;
			}

			if (!fail && (size.area() >= root.rc.area())) {
				size = Packeable2DRectWH(root.rc);
				min_func = f;
			}

			else if (fail && (_area > best_area)) {
				best_area = _area;
				best_func = f;
			}
			fail = false;
		}

		rects = order[min_func == -1 ? best_func : min_func];

		int clip_x = 0, clip_y = 0;
		Packeable2DNode* ret;

		root.reset(size);

		for (i = 0; i < rects.size(); ++i) {
			if (ret = root.insert(*rects[i])) {
				rects[i]->x = ret->rc.l;
				rects[i]->y = ret->rc.t;

				if (rects[i]->flipped) {
					rects[i]->flipped = false;
					rects[i]->flip();
				}

				clip_x = std::max(clip_x, ret->rc.r);
				clip_y = std::max(clip_y, ret->rc.b);
			}
			else
				return false;
		}

		for (int f = 0; f < funcs; ++f) {
			order[f].clear();
			order[f].shrink_to_fit();
		}

		out_width = clip_x;
		out_height = clip_y;
		return true;
	}

	Packeable2DRectWH::Packeable2DRectWH(const Packeable2DRectLTRB& rr) : w(rr.w()), h(rr.h()) {}
	Packeable2DRectWH::Packeable2DRectWH(const Packeable2DRectXYWH& rr) : w(rr.w), h(rr.h) {}
	Packeable2DRectWH::Packeable2DRectWH(int w, int h) : w(w), h(h) {}

	int Packeable2DRectWH::fits(const Packeable2DRectWH& r) const {
		if (w == r.w && h == r.h) return 3;
		if (h == r.w && w == r.h) return 4;
		if (w <= r.w && h <= r.h) return 1;
		if (h <= r.w && w <= r.h) return 2;
		return 0;
	}

	Packeable2DRectLTRB::Packeable2DRectLTRB() : l(0), t(0), r(0), b(0) {}
	Packeable2DRectLTRB::Packeable2DRectLTRB(int l, int t, int r, int b) : l(l), t(t), r(r), b(b) {}

	int Packeable2DRectLTRB::w() const {
		return r - l;
	}

	int Packeable2DRectLTRB::h() const {
		return b - t;
	}

	int Packeable2DRectLTRB::area() const {
		return w()*h();
	}

	int Packeable2DRectLTRB::perimeter() const {
		return 2 * w() + 2 * h();
	}

	void Packeable2DRectLTRB::w(int ww) {
		r = l + ww;
	}

	void Packeable2DRectLTRB::h(int hh) {
		b = t + hh;
	}

	Packeable2DRectXYWH::Packeable2DRectXYWH() : x(0), y(0) {}
	Packeable2DRectXYWH::Packeable2DRectXYWH(const Packeable2DRectLTRB& rc) : x(rc.l), y(rc.t) { b(rc.b); r(rc.r); }
	Packeable2DRectXYWH::Packeable2DRectXYWH(int x, int y, int w, int h) : x(x), y(y), Packeable2DRectWH(w, h) {}

	Packeable2DRectXYWH::operator Packeable2DRectLTRB() {
		Packeable2DRectLTRB rr(x, y, 0, 0);
		rr.w(w); rr.h(h);
		return rr;
	}

	int Packeable2DRectXYWH::r() const {
		return x + w;
	};

	int Packeable2DRectXYWH::b() const {
		return y + h;
	}

	void Packeable2DRectXYWH::r(int right) {
		w = right - x;
	}

	void Packeable2DRectXYWH::b(int bottom) {
		h = bottom - y;
	}

	int Packeable2DRectWH::area() {
		return w * h;
	}

	int Packeable2DRectWH::perimeter() {
		return 2 * w + 2 * h;
	}

	Packeable2D::Packeable2D(const Packeable2DRectLTRB& rr) : Packeable2DRectXYWH(rr), flipped(false) {}
	Packeable2D::Packeable2D(int x, int y, int width, int height) : Packeable2DRectXYWH(x, y, width, height), flipped(false) {}
	Packeable2D::Packeable2D() : flipped(false) {}

	void Packeable2D::flip() {
		flipped = !flipped;
		std::swap(w, h);
	}
} }