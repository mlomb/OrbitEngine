#include "OE/Graphics/API/Shader.hpp"

#include "OE/Application/Context.hpp"
#include "OE/System/File.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Graphics/API/ShaderReflection.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLShader.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DShader.hpp"
#endif
#if OE_VULKAN
	#include "OE/Platform/Vulkan/VKShader.hpp"
#endif

namespace OrbitEngine {	namespace Graphics {

	void Shader::attachFromFile(ShaderType type, std::string& path, bool binary)
	{
		std::vector<char> buffer = System::File::LoadFile(path);

		if (binary)
			attachFromBinary(type, buffer);
		else {
			std::string source(buffer.begin(), buffer.end());
			attachFromSource(type, source);
		}
	}

	ShaderReflection* Shader::getReflection()
	{
		return p_Reflection;
	}

	Shader::Shader()
	{
		p_Reflection = ShaderReflection::Create();
	}
	
	Shader::~Shader()
	{
		delete p_Reflection;
	}

	Shader* Shader::Create()
	{
		switch (Application::Context::GetCurrentAPI()) {
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			return new GLShader();
#endif
#if OE_D3D
		case DIRECT3D:
			return new D3DShader();
#endif
#if OE_VULKAN
		case VULKAN:
			return new VKShader();
#endif
		default:
			return nullptr;
		}
	}
} }