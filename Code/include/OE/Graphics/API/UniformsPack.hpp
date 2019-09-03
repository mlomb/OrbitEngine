#ifndef GRAPHICS_UNIFORMS_PACK_HPP
#define GRAPHICS_UNIFORMS_PACK_HPP

#include "OE/Config.hpp"
#include "OE/Misc/Log.hpp"
#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine {	namespace Graphics {
	template <class T>
	class UniformsPack {
	public:
		virtual void setData(const T& data) = 0;
		virtual void bind(unsigned int slot, ShaderType shader) const = 0;

		void bind(const std::string& name, Shader* shader) const;

		static UniformsPack<T>* Create();
	};
} }

#include "OE/Application/Context.hpp"
#include "OE/Graphics/API/ShaderReflection.hpp"
#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLUniformBuffer.hpp"
#endif
#if OE_D3D
	#include "OE/Platform/Direct3D/D3DConstantBuffer.hpp"
#endif

namespace OrbitEngine {	namespace Graphics {
	template<class T>
	inline void UniformsPack<T>::bind(const std::string& name, Shader* shader) const
	{
		shader->bind();
		for (const auto& p : shader->getReflections()) {
			for (auto& buff : p.second.buffers) {
				if(buff.name == name)
					bind(buff.slot, p.first);
			}
		}
	}

	template<class T>
	inline UniformsPack<T>* UniformsPack<T>::Create()
	{
		switch (Application::Context::GetCurrentAPI()) {
#if OE_OPENGL_ANY
		case OPENGL:
#if OE_OPENGL_ES
		case OPENGL_ES:
#endif
			return new GLUniformBuffer<T>();
#endif
#if OE_D3D
		case DIRECT3D:
			return new D3DConstantBuffer<T>();
#endif
		}

		OE_ASSERT(false);
		return 0;
	}
} }

#endif