#ifndef GRAPHICS_SURFACE_SHADER_HPP
#define GRAPHICS_SURFACE_SHADER_HPP

#include <map>

#include "OE/Graphics/ManagedShader.hpp"

namespace OrbitEngine { namespace Graphics {

	class SurfaceShader : public ManagedShader {
	public:
		SurfaceShader(const std::string& surfaceShaderPath);

		static SurfaceShader* Standard();
	private:
		static SurfaceShader* s_Standard;
		std::string m_SurfacePath;
	};
} }

#endif