#include "OE/Graphics/Bitmap.hpp"

#include "OE/Math/Math.hpp"

namespace OrbitEngine { namespace Graphics {

	unsigned char floatPixelToByte(float f) {
		return (unsigned char)(Math::clamp(256.0f * f, 0.0f, 255.0f));
	}

	template<> BitmapRGB ConvertBitmap(const Bitmap<unsigned char, 1>& input) {
		BitmapRGB result(input.width(), input.height());
		unsigned char v;
		for (int j = 0; j < input.height(); j++) {
			for (int i = 0; i < input.width(); i++) {
				v = input(i, j, 0);
				result(i, j, 0) = v;
				result(i, j, 1) = v;
				result(i, j, 2) = v;
			}
		}
		return result;
	}

	template<> BitmapRGB ConvertBitmap(const Bitmap<float, 1> & input) {
		BitmapRGB result(input.width(), input.height());
		float f;
		unsigned char v;
		for (int j = 0; j < input.height(); j++) {
			for (int i = 0; i < input.width(); i++) {
				v = floatPixelToByte(input(i, j, 0));
				result(i, j, 0) = v;
				result(i, j, 1) = v;
				result(i, j, 2) = v;
			}
		}
		return result;
	}

	template<> BitmapRGB ConvertBitmap(const Bitmap<float, 3> & input) {
		BitmapRGB result(input.width(), input.height());
		for (int j = 0; j < input.height(); j++) {
			for (int i = 0; i < input.width(); i++) {
				result(i, j, 0) = floatPixelToByte(input(i, j, 0));
				result(i, j, 1) = floatPixelToByte(input(i, j, 1));
				result(i, j, 2) = floatPixelToByte(input(i, j, 2));
			}
		}
		return result;
	}

} }
