#ifndef GRAPHICS_SHADER_HPP
#define GRAPHICS_SHADER_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

namespace OrbitEngine {	namespace Graphics {
	class ShaderReflection;

	enum ShaderType {
		UNKNOWN = 0,
		VERTEX,
		FRAGMENT, // or PIXEL
		GEOMETRY,
		TESS_CONTROL,
		TESS_EVAL
	};

	typedef std::map<ShaderType, std::string> ShaderSources;
	typedef std::set<std::string> ShaderDefinitions;

	class Shader {
	public:
		~Shader();

		static Shader* Create();

		void attachFromFile(ShaderType type, std::string& path, bool binary = false);
		ShaderReflection* getReflection();

		virtual void attachFromSource(ShaderType type, const std::string& source) = 0;
		virtual void attachFromBinary(ShaderType type, const std::vector<char>& binary) = 0;
		virtual void finalize() = 0;

		virtual void bind() const = 0;
		
	protected:
		ShaderReflection* p_Reflection;

		Shader();
	};
	
} }

#endif