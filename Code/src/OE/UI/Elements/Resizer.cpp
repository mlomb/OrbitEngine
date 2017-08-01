#include "OE/UI/Elements/Resizer.hpp"

#include "OE/Graphics/2D/PathRenderer2D.hpp"

namespace OrbitEngine {	namespace UI {
	Resizer::Resizer()
	{
		// direction
	}

	void Resizer::onRender(Graphics::PathRenderer2D& r2d)
	{
		r2d.beginPath();
		r2d.bindTexture(0);
		r2d.bindColor(Math::Color(1, 0, 0, 0.5));
		r2d.rect(getRenderPosition(), getRenderSize());
		r2d.fill();
	}
} }