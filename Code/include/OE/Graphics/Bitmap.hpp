#ifndef GRAPHICS_BITMAP_HPP
#define GRAPHICS_BITMAP_HPP

#include "OE/Graphics/FreeImage.hpp"
#include "OE/Graphics/API/Texture.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	/**
		@brief A 2D bitmap image with N channels of elements T.

		Example: filling a Bitmap with magenta and saving it to the disk

		@code
		Bitmap<unsigned char, 3> bitmap(128, 128); // or BitmapRGB

		for(int y = 0; y < bitmap.height(); y++) {
			for(int x = 0; x < bitmap.width(); x++) {
				bitmap(x, y, 0) = 255;
				bitmap(x, y, 1) = 0;
				bitmap(x, y, 2) = 255;
			}
		}

		bitmap.savePNG("magenta.png");
		@endcode
	*/
	template<typename T, unsigned int N = 1>
	class Bitmap {
	public:
		/// Creates an invalid bitmap
		Bitmap();
		/// Creates an empty WxH bitmap with \p N channels
		Bitmap(unsigned int w, unsigned int h);
		/**
			@brief Creates a WxH bitmap with \p N channels **copying** the pixels from \p source.

			\p source must have at least W*H*N elements
		*/
		Bitmap(unsigned int w, unsigned int h, T* source);
		/**
			@brief Creates a WxH bitmap with \p N channels **using** the pixels from \p source.
			The bitmap will **not** take ownership of the input pixels.

			\p source must have at least W*H*N elements
		*/
		Bitmap(T* source, unsigned int w, unsigned int h);
		/// copy constructor
		Bitmap(const Bitmap<T, N>& other);
		/// move constructor
		Bitmap(Bitmap<T, N>&& other) noexcept;
		/// destructor
		~Bitmap();

		/// returns whether the bitmap has valid data
		bool valid() const;
		/// width in pixels
		unsigned int width() const;
		/// height in pixels
		unsigned int height() const;
		/// bits per pixel
		unsigned int bpp() const;
		/// reference to the ith element at \p x, \p y
		T& operator()(int x, int y, int ith);
		/// const reference to the ith element at \p x, \p y
		const T& operator()(int x, int y, int ith) const;
		/// move assignment operator
		Bitmap<T, N>& operator=(Bitmap<T, N>&& other) noexcept;
		/// pixel data
		T* data() const;

		/**
			@brief Writes the input Bitmap at \p x, \p y.

			If the coordinates provided are outside bounds, the function will
			only write the pixels that remain inside the boundaries
		*/
		void write(const Bitmap<T, N>& input, int x, int y);
		/// Fill the entire bitmap with the provided elements
		void fill(const T input[N]);
		/// Clear the entire bitmap (sets all the pixels to zero)
		void clear();
		/// In-place flip vertically
		void flipVertically();
		/// In-place flip horizontally
		void flipHorizontally();
		/// In-place flip horizontally and vertically
		void flipBoth();
		/// Returns a rotated copy of the bitmap 90 degrees clockwise
		Bitmap<T, N> rotate90clockwise() const;
		/// Deletes the pixel data and makes the bitmap invalid
		void destroy();

		/**
			@brief Saves the bitmap as a PNG file.
			@warning Bitmaps with \p T other than unsigned char may be corrupted
		*/
		bool savePNG(const std::string& path) const;

		/**
			@brief Generate a Texture from the Bitmap
			@note Remind that the Texture and Bitmap will not be linked in any way and if you want to
				  update the texture you will have to generate a new texture with this method again
		*/
		Texture* toTexture(const TextureSampleProperties& sample_properties = TextureSampleProperties());

		/**
			@brief Loads an image into a Bitmap
			@note The image will be transformed to the requested format
		*/
		static Bitmap<T, N> Load(const std::string& path);

	protected:
		T* m_Pixels;
		unsigned int m_Width, m_Height;
		/// Determinates if the Bitmap object owns \p m_Pixels and it should delete them after its destruction
		bool m_Ownership;
	};

	typedef Bitmap<unsigned char, 3> BitmapRGB;
	typedef Bitmap<unsigned char, 4> BitmapRGBA;

	/**
		@brief Convert between bitmaps

		Example: convert a 8bit grayscale image into RGB

		@code
		Bitmap<unsigned char, 1> bitmap_grayscale(128, 128);

		// ... do something with bitmap_grayscale

		bitmap_grayscale.savePNG("image-8bit.png");

		BitmapRGB bitmap_rgb = ConvertBitmap(bitmap_grayscale); // or Bitmap<unsigned char, 3>

		bitmap_rgb.savePNG("image-24bit.png");
		@endcode
	*/
	template<typename T, unsigned int N, typename U, unsigned int M>
	Bitmap<U, M> ConvertBitmap(const Bitmap<T, N>& input) { return U::conversion_not_implemented; }
	/// Convert from unsigned char 8bit (grayscale) to unsigned char 24bit (RGB)
	template<> BitmapRGB ConvertBitmap(const Bitmap<unsigned char, 1> & input);
	/// Convert from unsigned char 8bit (grayscale) to unsigned char 32bit (RGBA)
	template<> BitmapRGBA ConvertBitmap(const Bitmap<unsigned char, 1> & input);
	/// Convert from unsigned char 24bit (RGB) to unsigned char 32bit (RGBA)
	template<> BitmapRGBA ConvertBitmap(const Bitmap<unsigned char, 3> & input);
	/// Convert from unsigned char 32bit (RGBA) to unsigned char 24bit (RGB) (drop alpha)
	template<> BitmapRGB ConvertBitmap(const Bitmap<unsigned char, 4> & input);
	/// Convert from float 8bit (grayscale) to unsigned char 24bit (RGB)
	template<> BitmapRGB ConvertBitmap(const Bitmap<float, 1> & input);
	/// Convert from float 24bit (RGB) to unsigned char 24bit (RGB)
	template<> BitmapRGB ConvertBitmap(const Bitmap<float, 3> & input);

	/// Convert a RGBA bitmap from BGRA to RGBA
	BitmapRGBA ConvertBGRAtoRGBA(const Bitmap<unsigned char, 4>& /* BGRA */ input);
	BitmapRGBA ConvertAlphaToWhiteRGBA(const Bitmap<unsigned char, 1>& /* Alpha8 */ input);

	template<typename T, unsigned int N> Bitmap<T, N>::Bitmap()
		: m_Pixels(NULL), m_Width(0), m_Height(0), m_Ownership(true)
	{
	}

	template<typename T, unsigned int N> Bitmap<T, N>::Bitmap(unsigned int w, unsigned int h)
		: m_Width(w), m_Height(h), m_Ownership(true)
	{
		m_Pixels = new T[w * h * N];
	}

	template<typename T, unsigned int N> Bitmap<T, N>::Bitmap(unsigned int w, unsigned int h, T* source)
		: Bitmap(w, h)
	{
		memcpy(m_Pixels, source, w * h * N * sizeof(T));
	}

	template<typename T, unsigned int N>
	inline Bitmap<T, N>::Bitmap(T* source, unsigned int w, unsigned int h)
		: m_Width(w), m_Height(h), m_Pixels(source), m_Ownership(false)
	{
	}

	template <typename T, unsigned int N> Bitmap<T, N>::Bitmap(const Bitmap<T, N>& other)
		 : Bitmap(other.m_Width, other.m_Height, other.m_Pixels)
	{
	}

	template <typename T, unsigned int N> Bitmap<T, N>::Bitmap(Bitmap<T, N>&& other) noexcept
		: m_Pixels(other.m_Pixels), m_Width(other.m_Width), m_Height(other.m_Height), m_Ownership(other.m_Ownership) {
		other.m_Pixels = NULL;
		other.m_Width = 0;
		other.m_Height = 0;
		other.m_Ownership = true;
	}

	template <typename T, unsigned int N> Bitmap<T, N>::~Bitmap() {
		if(m_Ownership && m_Pixels)
			delete[] m_Pixels;
	}

	template<typename T, unsigned int N> bool Bitmap<T, N>::valid() const {
		return m_Width > 0 && m_Height > 0 && m_Pixels != NULL;
	}

	template <typename T, unsigned int N> unsigned int Bitmap<T, N>::width() const {
		return m_Width;
	}

	template <typename T, unsigned int N> unsigned int Bitmap<T, N>::height() const {
		return m_Height;
	}

	template<typename T, unsigned int N> unsigned int Bitmap<T, N>::bpp() const {
		return 8 * N * sizeof(T);
	}

	template <typename T, unsigned int N> T& Bitmap<T, N>::operator()(int x, int y, int ith) {
		return *(m_Pixels + (N * (m_Width * y + x) + ith));
	}

	template<typename T, unsigned int N> const T& Bitmap<T, N>::operator()(int x, int y, int ith) const {
		return *(m_Pixels + (N * (m_Width * y + x) + ith));
	}

	template <typename T, unsigned int N> Bitmap<T, N>& Bitmap<T, N>::operator=(Bitmap<T, N>&& other) noexcept {
		if (this != &other) {
			if (m_Ownership && m_Pixels)
				delete[] m_Pixels;
			m_Pixels = other.m_Pixels;
			m_Width = other.m_Width;
			m_Height = other.m_Height;
			m_Ownership = other.m_Ownership;

			other.m_Pixels = NULL;
			other.m_Width = 0;
			other.m_Height = 0;
			other.m_Ownership = true;
		}
		return *this;
	}
	template<typename T, unsigned int N> T* Bitmap<T, N>::data() const {
		return m_Pixels;
	}

	template <typename T, unsigned int N> void Bitmap<T, N>::write(const Bitmap<T, N>& input, int x, int y) {
		int bx = std::min(m_Width, x + input.m_Width);
		int by = std::min(m_Height, y + input.m_Height);

		for (int py = std::max(0, y); py < by; py++) {
			for (int px = std::max(0, x); px < bx; px++) {
				for (int i = 0; i < N; i++) {
					operator()(px, py, i) = input(px - x, py - y, i);
				}
			}
		}
	}

	template<typename T, unsigned int N> void Bitmap<T, N>::fill(const T input[N]) {
		for (int y = 0; y < m_Height; y++) {
			for (int x = 0; x < m_Width; x++) {
				for (int i = 0; i < N; i++) {
					operator()(x, y, i) = input[i];
				}
			}
		}
	}

	template<typename T, unsigned int N> void Bitmap<T, N>::clear() {
		T arr[N];
		for (int i = 0; i < N; i++)
			arr[i] = 0;
		fill(arr);
	}

	template<typename T, unsigned int N> void Bitmap<T, N>::flipVertically() {
		for (int y = 0; y < m_Height / 2; y++) {
			for (int x = 0; x < m_Width; x++) {
				for (int i = 0; i < N; i++) {
					std::swap(operator()(x, y, i), operator()(x, m_Height - y - 1, i));
				}
			}
		}
	}

	template<typename T, unsigned int N> void Bitmap<T, N>::flipHorizontally() {
		for (int y = 0; y < m_Height; y++) {
			for (int x = 0; x < m_Width / 2; x++) {
				for (int i = 0; i < N; i++) {
					std::swap(operator()(x, y, i), operator()(m_Width - x - 1, y, i));
				}
			}
		}
	}

	template<typename T, unsigned int N> void Bitmap<T, N>::flipBoth() {
		for (int y = 0; y < m_Height / 2; y++) {
			for (int x = 0; x < m_Width; x++) {
				for (int i = 0; i < N; i++) {
					std::swap(operator()(x, y, i), operator()(m_Width - x - 1, m_Height - y - 1, i));
				}
			}
		}
	}

	template<typename T, unsigned int N> Bitmap<T, N> Bitmap<T, N>::rotate90clockwise() const {
		Bitmap<T, N> result(m_Height, m_Width);
		for (int y = 0; y < m_Height; y++) {
			for (int x = 0; x < m_Width; x++) {
				for (int i = 0; i < N; i++) {
					result(m_Height - y - 1, x, i) = operator()(x, y, i);
				}
			}
		}
		return result;
	}

	template<typename T, unsigned int N> void Bitmap<T, N>::destroy() {
		if (m_Ownership)
			delete m_Pixels;
		m_Pixels = NULL;
		m_Width = 0;
		m_Height = 0;
		m_Ownership = true;
	}

	template<typename T, unsigned int N> bool Bitmap<T, N>::savePNG(const std::string& path) const {
		return WritePNG(path, m_Width, m_Height, bpp(), (unsigned char*)m_Pixels);
	}

	template<typename T, unsigned int N> Texture* Bitmap<T, N>::toTexture(const TextureSampleProperties& sample_properties) {
		TextureProperties properties;
		properties.width = m_Width;
		properties.height = m_Height;
		properties.sampleProperties = sample_properties;

		if (std::is_same<T, unsigned char>::value) {
			properties.formatProperties.dataType = TextureDataType::UNSIGNED_BYTE;

			switch (N)
			{
			case 1: properties.formatProperties.format = TextureFormat::R8; break;
			case 2: properties.formatProperties.format = TextureFormat::RG8; break;
			case 3: properties.formatProperties.format = TextureFormat::RGB8; break;
			case 4: properties.formatProperties.format = TextureFormat::RGBA8; break;
			default:
				OE_LOG_WARNING("Unsupported bit depth");
				return nullptr;
			}
		}
		else {
			OE_LOG_WARNING("Unsupported data type");
			return nullptr;
		}

		return Texture::Create(properties, m_Pixels);
	}

	template<typename T, unsigned int N> Bitmap<T, N> Bitmap<T, N>::Load(const std::string& path) {
		unsigned int w, h, bpp;
		unsigned char* data = ReadImage(path, w, h, bpp, N == 4);
		if (data) {
			if (std::is_same<T, unsigned char>::value) {
				if (bpp / 8 == N) {
					Bitmap<T, N> bmp(w, h, data);
					delete data;
					return bmp;
				}
				else if (N == 3 && bpp == 32) { // image is RGBA but we need RGB
					BitmapRGBA tmp(data, w, h); // no copy
					BitmapRGB bmp = ConvertBitmap<unsigned char, 4, unsigned char, 3>(tmp);
					delete data;
					return Bitmap<T, N>(w, h, bmp.data()); // copy
				} else {
					OE_LOG_WARNING("Unsupported bit depth");
				}
			}
			else {
				OE_LOG_WARNING("Unsupported bitmap type");
			}
			delete data;
		}
		return Bitmap<T, N>();
	}
} }

#endif