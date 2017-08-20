#include "OE/Graphics/3D/SurfaceShader.hpp"

#include "OE/Graphics/ShaderLoader.hpp"

namespace OrbitEngine { namespace Graphics {

	SurfaceShader* SurfaceShader::s_Standard = 0;

	SurfaceShader::SurfaceShader(const std::string& surfaceShaderPath)
		: ManagedShader("Resources/Shaders/3DRender.oeshader"),
		m_SurfacePath(surfaceShaderPath)
	{
		std::string surfaceHLSL = ShaderLoader::ResolveShaderFile(surfaceShaderPath);

		for (auto& p : p_Sources) {
			std::string needle = "${surface}";
			size_t pos;
			if ((pos = p.second.find(needle)) != std::string::npos)
				p.second = p.second.replace(p.second.find(needle), needle.size(), surfaceHLSL);
		}
	}

	SurfaceShader* SurfaceShader::Standard()
	{
		if (s_Standard == 0)
			s_Standard = new SurfaceShader("Resources/Shaders/Standard.oesurface");
		return s_Standard;
	}
} }