#include "OE/Graphics/API/Shader.hpp"

#include "OE/Application/Context.hpp"
#include "OE/System/File.hpp"
#include "OE/Misc/Log.hpp"

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

	void Shader::attachFromFile(ShaderType type, const std::string& path, bool binary)
	{
		std::vector<char> buffer = System::LoadFile(path, binary);

		if (binary)
			attachFromBinary(type, buffer);
		else {
			std::string source(buffer.begin(), buffer.end());
			attachFromSource(type, source);
		}
	}

	Shader::Shader()
	{

	}
	
	Shader::~Shader()
	{

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
			return 0;
		}
	}

	Shader* Shader::LoadInternalShader(const char* name)
	{
		std::string patformId;

		switch (Application::Context::GetCurrentAPI())
		{
#if OE_OPENGL
		case RenderAPI::OPENGL:
			patformId = "GL";
			break;
#endif
#if OE_OPENGL_ES
		case RenderAPI::OPENGL_ES:
			patformId = "GLES";
			break;
#endif
#if OE_D3D
		case RenderAPI::DIRECT3D:
			patformId = "D3D11";
			break;
#endif
		}

		Shader* shader = Create();
		shader->attachFromFile(ShaderType::VERTEX, "Resources/Shaders/" + std::string(name) + "_" + patformId + ".vert");
		shader->attachFromFile(ShaderType::FRAGMENT, "Resources/Shaders/" + std::string(name) + "_" + patformId + ".frag");
		shader->finalize();

		return shader;
	}
} }