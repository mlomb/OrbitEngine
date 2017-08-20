#ifndef GRAPHICS_MANAGED_SHADER_HPP
#define GRAPHICS_MANAGED_SHADER_HPP

#include "OE/Graphics/API/Shader.hpp"

#include <map>
#include <vector>
#include <set>
#include <string>

namespace OrbitEngine { namespace Graphics {
	class ManagedShader {
	public:
		ManagedShader(const std::string& path);
		~ManagedShader();

		Shader* requestShader(ShaderDefinitions definitions);

	private:
		typedef std::map<ShaderDefinitions, Shader*> ShaderInstances;

		std::string m_Path;
		ShaderInstances m_Instances;

	protected:
		ShaderSources p_Sources;
	};
} }

#endif