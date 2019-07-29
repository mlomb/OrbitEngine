#include "OE/Graphics/ShaderLoader.hpp"

#include <algorithm>

#include "OE/Graphics/ShaderCompiler.hpp"
#include "OE/System/File.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	
	Shader* ShaderLoader::LoadInternalShader(const std::string& name)
	{
		return LoadRawShader("Resources/Shaders/" + name + ".oeshader");
	}

	Shader* ShaderLoader::LoadRawShader(const std::string& path)
	{
		ShaderSources sources = LoadShaderSources(path);

		Shader* shader = Shader::Create();
		for (const auto& kv : sources)
			shader->attachFromSource(kv.first, ShaderCompiler::CrossCompileHLSL(kv.second, kv.first));
		shader->finalize();

		return shader;
	}

	ShaderSources ShaderLoader::LoadShaderSources(const std::string& path)
	{
		// Resolve includes
		std::string source = ResolveShaderFile(path);

		/* Now we split the different programs VERTEX, FRAMGENT, etc */
		ShaderSources sources;
		ShaderType currentType = ShaderType::UNKNOWN;
		std::stringstream sstr(source);
		std::string buffer, line;

		while (std::getline(sstr, line))
		{
			if (line.find("#shadertype") == 0) {
				std::string type = line.substr(12, line.size()); // +1 for the space
				if (currentType != ShaderType::UNKNOWN) {
					sources.insert(std::make_pair(currentType, buffer));
					currentType = ShaderType::UNKNOWN;
					buffer = std::string();
				}
				if (type.find("VERTEX") != std::string::npos)
					currentType = ShaderType::VERTEX;
				else if (type.find("FRAGMENT") != std::string::npos)
					currentType = ShaderType::FRAGMENT;
				else if (type.find("GEOMETRY") != std::string::npos)
					currentType = ShaderType::GEOMETRY;
				else if (type.find("TESS_CONTROL") != std::string::npos)
					currentType = ShaderType::TESS_CONTROL;
				else if (type.find("TESS_EVAL") != std::string::npos)
					currentType = ShaderType::TESS_EVAL;
			}
			else {
				buffer.append(line + "\r\n");
			}
		}
		if (currentType != ShaderType::UNKNOWN)
			sources.insert(std::make_pair(currentType, buffer));

		return sources;
	}

	std::string ShaderLoader::ResolveShaderFile(const std::string& path)
	{
		static const char* shaderLineJump = "\r\n";

		std::string directory = path.substr(0, path.find_last_of('/'));

		std::string fileContent = System::File::ReadAsString(path);
		if (fileContent.size() == 0) {
			OE_LOG_FATAL("Can't load shader file '" + path + "'.");
			return "/* Couldn't load file (" + path + ") */";
		}

		std::istringstream file(fileContent);

		std::stringstream buffer;
		std::string line;
		while (std::getline(file, line))
		{
			// Remove the jumpline that some platforms add
			line.erase(std::remove_if(line.begin(), line.end(),
				[](char c) { return (c == '\r' || c == '\t' || c == '\n' || c == '\0'); }), line.end());

			if (line.find("#include") == 0) {
				std::string includeFilePath = line.substr(9, line.size()); // +1 for the space
				buffer << ResolveShaderFile(directory + "/" + includeFilePath) << shaderLineJump;
			}
			else
				buffer << line << shaderLineJump;
		}

		return buffer.str();
	}
} }
