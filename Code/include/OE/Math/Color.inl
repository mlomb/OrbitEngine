#include "Color.hpp"
#ifndef COLOR_INL
#define COLOR_INL

#include "OE/Math/Color.hpp"

namespace OrbitEngine { namespace Math {


	inline Color3f::Color3f()
		: Vec3f()
	{
	}

	inline Color3f::Color3f(const Vec3f& color)
		: Vec3f(color.r, color.g, color.b)
	{
	}

	inline Color3f::Color3f(const Color3f& color)
		: Vec3f(color.r, color.g, color.b)
	{
	}

	inline Color3f::Color3f(const Vec4f& color)
		: Vec3f(color.r, color.g, color.b)
	{
	}

	inline Color3f::Color3f(const Color4f& color)
		: Vec3f(color.r, color.g, color.b)
	{
	}

	inline Color3f::operator Vec3f() const
	{
		return Vec3f(r, g, b);
	}

	inline Color4f::Color4f()
		: Vec4f()
	{
	}
	inline Color4f::Color4f(const Vec3f& color)
		: Vec4f(color.r, color.g, color.b, 1.0f) // full alpha
	{
	}

	inline Color4f::Color4f(const Color3f& color)
		: Vec4f(color.r, color.g, color.b, 1.0f) // full alpha
	{
	}

	inline Color4f::Color4f(const Color3f& color, float alpha)
		: Vec4f(color.r, color.g, color.b, alpha)
	{
	}

	inline Color4f::Color4f(const Vec4f& color)
		: Vec4f(color.r, color.g, color.b, color.a)
	{
	}

	inline Color4f::Color4f(const Color4f& color)
		: Vec4f(color.r, color.g, color.b, color.a)
	{
	}

	inline Color4f::operator Vec4f() const
	{
		return Vec4f(r, g, b, a);
	}
} }

#endif