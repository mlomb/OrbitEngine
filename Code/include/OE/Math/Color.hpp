#ifndef MATH_COLOR_HPP
#define MATH_COLOR_HPP

#include "OE/Math/Vec3.hpp"
#include "OE/Math/Vec4.hpp"

namespace OrbitEngine { namespace Math {

	struct Color3f;
	struct Color4f;

	/// RGB Color (0-1)
	struct Color3f : public Vec3f {
		using Vec3f::Vec3;

		Color3f();
		Color3f(const Vec3f& color);
		Color3f(const Color3f& color);
		Color3f(const Vec4f& color);
		Color3f(const Color4f& color);

		operator Vec3f() const;
	};

	/// RGBA Color (0-1)
	struct Color4f : public Vec4f {
		using Vec4f::Vec4;

		Color4f();
		Color4f(const Vec3f& color);
		Color4f(const Color3f& color);
		Color4f(const Color3f& color, float alpha);
		Color4f(const Vec4f& color);
		Color4f(const Color4f& color);

		operator Vec4f() const;
	};

	namespace Color {
		/// Solid black color (0, 0, 0)
		static const Color3f Black = Color3f(0, 0, 0);
		/// Solid white color (1, 1, 1)
		static const Color3f White = Color3f(1, 1, 1);
		/// Solid red color (1, 0, 0)
		static const Color3f Red = Color3f(1, 0, 0);
		/// Solid green color (0, 1, 0)
		static const Color3f Green = Color3f(0, 1, 0);
		/// Solid blue color (0, 0, 1)
		static const Color3f Blue = Color3f(0, 0, 1);
		/// Solid magenta color (1, 0, 1)
		static const Color3f Magenta = Color3f(1, 0, 1);
		/// Transparent color (0, 0, 0, 0)
		static const Color4f Transparent = Color4f(0, 0, 0, 0);

		/**
			@brief Create an RGB color from HSL
			@param[in] h Hue ∈ [0, 360]
			@param[in] s Saturation ∈ [0, 1]
			@param[in] l Lightness ∈ [0, 1]
		*/
		Color3f FromHSL(int h, float s, float l);
	}
} }

#include "OE/Math/Color.inl"

#endif