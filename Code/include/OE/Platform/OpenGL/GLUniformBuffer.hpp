#ifndef GRAPHICS_GLUNIFORM_BUFFER_HPP
#define GRAPHICS_GLUNIFORM_BUFFER_HPP

#include "OE/Platform/OpenGL/GLBuffer.hpp"

#include "OE/Graphics/API/UniformsPack.hpp"
#include "OE/Graphics/API/Shader.hpp"

#include "OE/Platform/OpenGL/OpenGL.hpp"

namespace OrbitEngine {	namespace Graphics {
	template <class T>
	class GLUniformBuffer : public UniformsPack<T>, public GLBuffer {
	public:
		GLUniformBuffer();

		void setData(T& data) override;
		void bind(unsigned int slot, ShaderType shader) const override;
	};

	template<class T>
	inline GLUniformBuffer<T>::GLUniformBuffer() : GLBuffer(GL_UNIFORM_BUFFER, GL_STREAM_DRAW) {
		GLBuffer::bind();
		GLBuffer::setData(sizeof(T), 0);
	}

	template<class T>
	inline void GLUniformBuffer<T>::setData(T& data)
	{
		GLBuffer::setData(sizeof(T), &data);
	}

	template<class T>
	inline void GLUniformBuffer<T>::bind(unsigned int slot, ShaderType shaderType) const
	{
		if (slot > GL_MAX_UNIFORM_BUFFER_BINDINGS) {
			OE_LOG_WARNING("Max slot uniform buffer exceeded!");
			return;
		}

		glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_ID);
	}
} }

#endif