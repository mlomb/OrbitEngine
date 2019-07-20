#include "OE/Math/Color.hpp"

namespace OrbitEngine { namespace Math {

	float HueToRGB(float p, float q, float t) {
		if (t < 0.0f) t += 1.0f;
		if (t > 1.0f) t -= 1.0f;
		if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
		if (t < 1.0f / 2.0f) return q;
		if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
		return p;
	}

	Color FromHSL(int h, float s, float l) {
		if (s < 1e-5) { // achromatic
			return Color(l, l, l, 1.0f);
		}

		float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
		float p = 2.0f * l - q;
		float r = (float)h / 360.0f;

		return Color(
			HueToRGB(p, q, r + (1.0f / 3.0f)),
			HueToRGB(p, q, r),
			HueToRGB(p, q, r - (1.0f / 3.0f)),
			1.0f
		);
	}

} }