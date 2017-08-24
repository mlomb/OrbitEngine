#include "OE/Graphics/3D/Light.hpp"

namespace OrbitEngine { namespace Graphics {
	
	/* Point Light */

	ShaderDefinitions PointLight::getRequiredDefinitions()
	{
		// TODO concat with Light::getRequiredDefinitions
		return { "LIGHT_POINT" };
	}

	ShaderDefinitions DirectionalLight::getRequiredDefinitions()
	{
		// TODO concat with Light::getRequiredDefinitions
		return { "LIGHT_DIRECTIONAL" };
	}
} }