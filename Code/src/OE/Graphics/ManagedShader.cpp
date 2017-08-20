#include "OE/Graphics/ManagedShader.hpp"

#include "OE/Misc/Log.hpp"
#include "OE/Graphics/ShaderLoader.hpp"
#include "OE/Graphics/ShaderCompiler.hpp"

namespace OrbitEngine { namespace Graphics {

	ManagedShader::ManagedShader(const std::string& path)
		: m_Path(path)
	{
		m_Sources = ShaderLoader::LoadShaderSources(path);
	}

	ManagedShader::~ManagedShader()
	{
		m_Sources.clear();
		m_Instances.clear();
	}

	Shader* ManagedShader::requestShader(ShaderDefinitions definitions)
	{
		Shader* shader;

		ShaderInstances::iterator it = m_Instances.find(definitions);
		if (it != m_Instances.end())
			return it->second;
		else {
			// Create the shader with those definitions
			shader = Shader::Create();

			std::string strDefinitions;
			for (std::string definition : definitions)
				strDefinitions += "#define " + definition + "\r\n";

			for (const auto& kv : m_Sources) {
				std::string hlslSource = strDefinitions + kv.second;
				shader->attachFromSource(kv.first, ShaderCompiler::CrossCompileHLSL(hlslSource, kv.first));
			}

			shader->finalize();

			m_Instances.insert(std::make_pair(definitions, shader));
		}

		return shader;
	}
} }