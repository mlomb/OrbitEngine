#ifndef MATH_COLOR_GRADIENT_HPP
#define MATH_COLOR_GRADIENT_HPP

#include "OE/Math/Color.hpp"
#include "OE/Math/Gradient.hpp"

namespace OrbitEngine { namespace Math {
	
	/**
		@brief Color gradient

		If no alpha stops are provided, the alpha value will be set to 1.
	*/
	class ColorGradient {
	public:
		ColorGradient();

		void reset();
		void addAlphaStop(float time, float alpha);
		void addColorStop(float time, const Color3f& color);
		Color4f evaluate(float time);

	private:
		Gradient<Color3f> m_ColorGradient;
		Gradient<float> m_AlphaGradient;
	};

} }

#endif