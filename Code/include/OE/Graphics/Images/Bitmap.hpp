#ifndef GRAPHICS_BITMAP_HPP
#define GRAPHICS_BITMAP_HPP

namespace OrbitEngine { namespace Graphics {
	/// A 2D bitmap image with N channels of elements T
	template<typename T, unsigned int N = 1>
	class Bitmap {
	public:
		/// Creates an invalid bitmap
		Bitmap();
		/// Creates an empty WxH bitmap with N channels
		Bitmap(unsigned int w, unsigned int h);
		/// Creates a WxH bitmap with N channels **copying** the pixels from the source. \p source must have at least W*H*N elements
		Bitmap(unsigned int w, unsigned int h, T* source);
		~Bitmap();

		/// Bitmap width in pixels
		unsigned int width() const;
		/// Bitmap height in pixels
		unsigned int height() const;

		/*
		/// @param[in] x0,y0 first pair of coordinates to crop
		/// @param[in] x1,y1 second pair of coordinates to crop. \p x1 > \p x0 and \p y1 > \p y0
		/// @return Cropped view of the bitmap
		CroppedBitmap crop(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
		*/

		/// Writes the input Bitmap at \p x, \p y
		void write(const Bitmap<T, N>& input, int x, int y);

	private:
		T* m_Pixels;
		unsigned int m_Width, m_Height;
	};

	template<typename T, unsigned int N> Bitmap<T, N>::Bitmap()
		: m_Pixels(NULL), m_Width(0), m_Height(0)
	{
	}

	template<typename T, unsigned int N> Bitmap<T, N>::Bitmap(unsigned int w, unsigned int h)
		: m_Width(width), m_Height(height)
	{
		m_Pixels = new T[w * h * N];
	}

	template<typename T, unsigned int N> Bitmap<T, N>::Bitmap(unsigned int w, unsigned int h, T* source)
		: Bitmap(w, h)
	{
		memcpy(m_Pixels, source, w * h * N * sizeof(T));
	}

	template <typename T, unsigned int N> Bitmap<T, N>::~Bitmap() {
		delete[] pixels;
	}

	template <typename T, unsigned int N> unsigned int Bitmap<T, N>::width() const {
		return m_Width;
	}

	template <typename T, unsigned int N> unsigned int Bitmap<T, N>::height() const {
		return m_Height;
	}

	template <typename T, unsigned int N> void Bitmap<T, N>::write(const Bitmap<T, N>& input, int x, int y) {
		int px = max(0, x);
		int py = max(0, y);
		int bx = min(m_Width, x + input.m_Width);
		int by = min(m_Height, y + input.m_Height);

		for (; py < by; py++) {
			for (; px < bx; px++) {
				for (int i = 0; i < N; i++) {
					m_Pixels[N * (py * m_Width + px) + i] = input.m_Pixels[N * ((py - y) * m_Width + (px - x)) + i];
				}
			}
		}
	}
} }

#endif