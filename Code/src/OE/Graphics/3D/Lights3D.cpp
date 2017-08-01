#include "OE/Graphics/3D/Lights3D.hpp"

namespace OrbitEngine { namespace Graphics {
	PointLight3D::PointLight3D()
	{
		data.fallofs.x = 1.0f;
		data.fallofs.y = 0.022f;
		data.fallofs.z = 0.0019f;
	}
} }