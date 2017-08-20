#include "OE/Graphics/API/ShaderReflection.hpp"

#include "OE/Graphics/API/Shader.hpp"
#include "OE/Application/Context.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLShaderReflection.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DShaderReflection.hpp"
#endif

namespace OrbitEngine {	namespace Graphics {
	ShaderReflection::~ShaderReflection()
	{
	}
	ShaderReflection::ShaderReflection()
	{
	}

	std::vector<ShaderBufferReflection> ShaderReflection::getBuffers(const std::string& name) const
	{
		std::vector<ShaderBufferReflection> result;

		for (const ShaderBufferReflection& buffer : m_Buffers) {
			if (buffer.name == name)
				result.push_back(buffer);
		}

		return result;
	}

	std::vector<ShaderBufferReflection> ShaderReflection::getAllBuffers() const
	{
		return m_Buffers;
	}

	void ShaderReflection::addBuffer(ShaderBufferReflection buffer)
	{
		m_Buffers.push_back(buffer);
	}

	ShaderReflection* ShaderReflection::Create()
	{
		switch (Application::Context::GetCurrentAPI()) {
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			return new GLShaderReflection();
#endif
#if OE_D3D
		case DIRECT3D:
			return new D3DShaderReflection();
#endif
		}
	}
} }