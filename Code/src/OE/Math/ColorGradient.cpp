#include "OE/Math/ColorGradient.hpp"

namespace OrbitEngine { namespace Math {

	ColorGradient::ColorGradient()
	{
	}

	void ColorGradient::reset()
	{
		m_AlphaGradient.reset();
		m_ColorGradient.reset();
	}

	void ColorGradient::addAlphaStop(float time, float alpha)
	{
		m_AlphaGradient.addStop(time, alpha);
	}

	void ColorGradient::addColorStop(float time, const Color3f& color)
	{
		m_ColorGradient.addStop(time, color);
	}

	Color4f ColorGradient::evaluate(float time)
	{
		return Color4f(
			m_ColorGradient.evaluate(time),
			m_AlphaGradient.empty() ? 1.0f : m_AlphaGradient.evaluate(time)
		);
	}
} }
