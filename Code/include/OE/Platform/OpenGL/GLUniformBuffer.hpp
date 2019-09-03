#ifndef GRAPHICS_GLUNIFORM_BUFFER_HPP
#define GRAPHICS_GLUNIFORM_BUFFER_HPP

#include "OE/Platform/OpenGL/GLBuffer.hpp"
#include "OE/Platform/OpenGL/GLContext.hpp"

#include "OE/Graphics/API/UniformsPack.hpp"
#include "OE/Graphics/API/Shader.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine {	namespace Graphics {
	template <class T>
	class GLUniformBuffer : public UniformsPack<T>, public GLBuffer {
	public:
		GLUniformBuffer();
		~GLUniformBuffer();

		void setData(const T& data) override;
		void bind(unsigned int slot, ShaderType shader) const override;

	private:
		// internal copy of the data in case UBOs are not available
		T* m_Data;
	};

	template<class T>
	inline GLUniformBuffer<T>::GLUniformBuffer() : GLBuffer(GL_UNIFORM_BUFFER, GL_STREAM_DRAW), m_Data(NULL) {
		GLBuffer::bind();
		GLBuffer::setData(sizeof(T), 0);
	}

	template<class T>
	inline GLUniformBuffer<T>::~GLUniformBuffer()
	{
		if(m_Data)
			delete m_Data;
	}

	template<class T>
	inline void GLUniformBuffer<T>::setData(const T& data)
	{
		Application::priv::GLContext* glctx = Application::priv::GLContext::GetCurrent();
		if (glctx->getInfo().ubo_support) {
			GLBuffer::setData(sizeof(T), &data);
		}
		else {
			// keep a copy to the data
			// don't use the GLBuffer
			if (m_Data == NULL)
				m_Data = static_cast<T*>(malloc(sizeof(T)));
			memcpy(m_Data, &data, sizeof(T));
		}
	}

	template<class T>
	inline void GLUniformBuffer<T>::bind(unsigned int slot, ShaderType shaderType) const
	{
		Application::priv::GLContext* glctx = Application::priv::GLContext::GetCurrent();
		if (glctx->getInfo().ubo_support) {
			if (slot > GL_MAX_UNIFORM_BUFFER_BINDINGS) {
				OE_LOG_WARNING("Max slot uniform buffer exceeded!");
				return;
			}

			glBindBufferBase(GL_UNIFORM_BUFFER, (GLuint)slot, m_ID);
		}
		else {
			// TODO: Bind individual uniforms
		}
	}
} }

#endif